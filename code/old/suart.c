#include "main.h"
#include "suart.h"

volatile uint16_t srx_wait_flag;

//{{{ Constants and static variables
#define vu8(x)  (*(volatile u8*)&(x))

#define BIT_TIME    (uint16_t) (F_CPU * 1.0 / SUART_BAUDRATE + 0.5)

#define TX_HIGH     (1<<COM1A1^1<<COM1A0)
#define TX_LOW      (TX_HIGH^1<<COM1A0)
#define TX_OUT      TCCR1A								// use compare output mode

#define ROLLOVER(x, max)  x = ++x >= max ? 0 : x 		// count up and wrap around
// Äquivalent: if(++x >= max) x=0; // man achte auf das Inkrementieren von x

static uint8_t stx_buff[STX_SIZE];
static volatile uint8_t stx_in;
static volatile uint8_t stx_out;
static uint8_t stx_data;
static uint8_t stx_state;


static uint8_t srx_buff[SRX_SIZE];
static volatile uint8_t srx_in;
static volatile uint8_t srx_out;
static volatile uint8_t srx_data;
static volatile uint8_t srx_state;
//}}}


//{{{
void suartInit(void)
{
	uint8_t sreg = SREG;
	cli();
	OCR1A = BIT_TIME - 1;
	TCCR1A = TX_HIGH;									// set OC1A high, T1 mode 4
	TCCR1B = 1<<ICNC1^1<<WGM12^1<<CS10;					// noise canceler, 1-0 transition,
	// CLK/1, T1 mode 4 (CTC)
	TCCR1C = 1<<FOC1A;
	stx_state = 0;
	stx_in = 0;
	stx_out = 0;
	srx_in = 0;
	srx_out = 0;
	
	STXD_DDR = 1;										// output enable
	TIFR1 = 1<<ICF1;									// clear pending interrupt
	TIMSK1 = 1<<ICIE1^1<<OCIE1A;						// enable tx and wait for start
	SREG=sreg;
}
//}}}

/**************************** Send functions *****************************/

//{{{
static inline void set_srx_wait_flag(uint8_t value)
{
	uint8_t sreg = SREG;
	cli();
	srx_wait_flag = value;
	SREG = sreg;
}
//}}}

//{{{
void sputc(uint8_t c)
{	// Transmit one Byte: SoftuartPUTChar
	uint8_t i = stx_in;

	ROLLOVER(i, STX_SIZE);
	stx_buff[stx_in] = ~c;								// complement for stop bit after data
	//while( i == (*(volatile uint8_t *)(stx_out)) ;		// until at least one byte free
	// stx_out modified by interrupt !
	while( i == stx_out) ;		// until at least one byte free
	stx_in = i;
}
//}}}

//{{{
void sputa(uint8_t *a, uint8_t length)
{	// Transmit a whole Array using a Buffer
	for(uint8_t i=0; i<length; i++)
	{
		sputc(a[i]);
	}
}
//}}}

//{{{

//void uputla(uint8_t *a, uint16_t length)
//{
//	for(uint16_t i=0; i<length; i++)
//	{
//		uputchar(a[i]);
//	}
//}

//void uputs_(uint8_t *s)								// transmit string from SRAM
//{
//	while( *s ) uputchar(*s++);
//}


uint8_t skbhit(void)										// check, if rx buffer not empty
{
	//~ return srx_out ^ (*(volatile uint8_t *)(srx_in);	// this pointer trick makes the access volatile
	return srx_out ^ srx_in;	// srx_in changed by interrupt ! 
}


uint8_t sgetc_with_timeout(uint8_t* received_byte)									// wait until byte received with timeout
{
	set_srx_wait_flag(100);
	while( !skbhit())
	{
		uint8_t sreg=SREG;
		cli();
		if(srx_wait_flag == 0)
		{
			SREG=sreg;
			return FAILURE;
		}
		SREG=sreg;
	}									// until at least one byte in
	*received_byte = srx_buff[srx_out];							// get byte
	ROLLOVER(srx_out, SRX_SIZE);
	return SUCCESS;
}
//}}}

/****************************** Interrupts *******************************/

//{{{
ISR(TIMER1_CAPT_vect)									// start detection
{
	int16_t i = ICR1 - BIT_TIME / 2;					// scan at 0.5 bit time

	__asm__ volatile ("" ::);							// it helps, but why ? Remove useless R18/R19, avoid disoptimization.

	if( i < 0 ) i += BIT_TIME;							// wrap around
	OCR1B = i;
	srx_state = 10;
	TIFR1 = 1<<OCF1B;									// clear pending interrupt
	if( SRXD_PIN == 0 )									// still low
		TIMSK1 = 1<<OCIE1A^1<<OCIE1B;					// wait for first bit
}
//}}}


//{{{
ISR(TIMER1_COMPB_vect)									// receive data bits
{
	uint8_t i;

	switch( --srx_state )
	{
		case 9:
			if( SRXD_PIN == 0 )							// start bit valid
				return;

			break;

		default:
			//{{{

			i = srx_data >> 1;						// LSB first
			if( SRXD_PIN == 1 ) i |= 0x80;			// data bit = 1
			srx_data = i;
			//}}}
			return;

		case 0:
			if( SRXD_PIN == 1 )							// stop bit valid
			{
				//{{{

				i = srx_in;
				ROLLOVER(i, SRX_SIZE);
				if( i != srx_out )						// no buffer overflow
				{
					srx_buff[srx_in] = srx_data;
					srx_in = i;							// advance in pointer
				}
				//}}}
			}
			TIFR1 = 1<<ICF1;							// clear pending interrupt
	}
	TIMSK1 = 1<<ICIE1^1<<OCIE1A;						// enable next start
}
//}}}

//{{{
ISR(TIMER1_COMPA_vect)									// transmit data bits
{
	if( stx_state )
	{
		stx_state--;
		TX_OUT = TX_HIGH;
		if( stx_data & 1 )								// lsb first
			TX_OUT = TX_LOW;
		stx_data >>= 1;
		return;
	}
	if( stx_in != stx_out )								// next byte to sent
	{
		stx_data = stx_buff[stx_out];
		ROLLOVER(stx_out, STX_SIZE);
		stx_state = 9;									// 8 data bits + stop
		TX_OUT = TX_LOW;								// start bit
	}
}
//}}}
