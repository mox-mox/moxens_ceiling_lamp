#include "uart.h"
#include "main.h"
#include "uart.h"
#include "timer.h"

// Baud Berechnungen
//#define BAUDDIVIDER ((uint16_t)(F_CPU / UART_BAUDRATE / 16-0.5))




//static uint8_t urx_buff[URX_SIZE];
//volatile uint8_t urx_in;
//volatile uint8_t urx_out;
static uint8_t utx_buff[UTX_SIZE];
static volatile uint8_t utx_in;
static volatile uint8_t utx_out;

static uint8_t urx_buff[URX_SIZE];
static volatile uint8_t urx_in;
static volatile uint8_t urx_out;

#define ROLLOVER(x, max)  x = ++x >= max ? 0 : x // count up and wrap around

//{{{
void uartInit()
{
	//~ uint16_t bauddivider = ((uint16_t)((F_CPU / baudrate / 16)-0.5));
	//~ UBRR0H = (bauddivider >> 8);
	//~ UBRR0L = bauddivider;															// set baud rate
	UBRR0 = UBRR_VAL;
	UCSR0A = 0;																		// no U2X, MPCM
	UCSR0C = 1<<UCSZ01^1<<UCSZ00;													// 8 Bit
	UCSR0B = 1<<RXEN0^																// enable RX
		   //1<<TXEN0^																// DO NOT enable TX
	         1<<RXCIE0;																// enable RX interrupt
	urx_in = urx_out = 0;															// set buffer empty
	utx_in = utx_out = 0;
	timerInit();																	// ugetc_with_timeout() braucht timer0
}
//}}}


/************************** Receive functions ****************************/

//{{{
static inline void set_rx_wait_flag(uint8_t value)
{
	uint8_t sreg = SREG;
	cli();
	rx_wait_flag = value;
	SREG = sreg;
}
//}}}


//{{{
uint8_t ukbhit(void)
{
	return urx_out ^ urx_in;															// urx_in modified by interrupt !
}
//}}}

//
////{{{
//uint8_t ugetc(void)
//{
//	uint8_t data;
//
//	while( !ukbhit()) ;																	// until at least one byte in
//	data = urx_buff[urx_out];															// get byte
//	ROLLOVER(urx_out, URX_SIZE);
//	return data;
//}
////}}}
//

//{{{
uint8_t ugetc_with_timeout(uint8_t* received_byte)
{
	while(!(UCSR0B & (1<<RXEN0))); 														// erstmal warten, bis der Receiveteil überhaupt an ist
	// Das muss >> 4 sein, weil die Servos warten, bevor sie eine Antwort senden
	set_rx_wait_flag(100);																// Warteflagge setzen, sodass wir 1-2 Bytezeiten warten
	while( !ukbhit()) 																	// until at least one byte in
	{
		uint8_t sreg=SREG;																// Der Vergleich sollte unter Interruptausschluss sein
		cli();
		if(rx_wait_flag == 0)															// Falls nach 1-2 Bytezeiten noch kein Byte angekommen ist
		{
			SREG=sreg;
			return FAILURE;																// brechen wir das Ganze ab, und kehren erfolglos zurück.
		}
		SREG=sreg;
	}
	*received_byte = urx_buff[urx_out];													// get byte
	ROLLOVER(urx_out, URX_SIZE);
	return SUCCESS;
}
//}}}


//{{{
//uint8_t utx_ready(void)
//{
//	uint8_t i = utx_in;
//
//	ROLLOVER(i, UTX_SIZE);
//	return utx_out ^ i;																	// 0 = busy
//}
//}}}


/*************************** helper functions ****************************/

uint8_t utx_buffer_filled()
{
	return utx_in != utx_out;
}


//{{{
void to_rx()																			// transmit abschalten, receive anschalten
{
	setLED_tail(1,1);	// TODO: toggle
	uint8_t sreg = SREG;
	cli();
	SBIT( UCSR0B, UDRIE0) = 0;
	SBIT( UCSR0B, TXEN0 ) = 0;

	SBIT( UCSR0B, RXEN0 ) = 1;
	SBIT( UCSR0B, RXCIE0) = 1;
	SREG = sreg;
}
//}}}


//{{{
void to_tx()																			// receive abschalten, transmit anschalten
{
	setLED_tail(1,0);	// TODO: toggle
	uint8_t sreg = SREG;
	cli();
	SBIT( UCSR0B, RXCIE0) = 0;
	SBIT( UCSR0B, RXEN0 ) = 0;

	SBIT( UCSR0B, TXEN0 ) = 1;
	SBIT( UCSR0B, UDRIE0) = 1;
	SREG = sreg;
}
//}}}


/**************************** Send functions *****************************/

//{{{
void uputc(uint8_t c)
{
	uint8_t i = utx_in;

	ROLLOVER(i, UTX_SIZE);
	utx_buff[utx_in] = c;
	while( i == utx_out) ;																// until at least one byte free
	// utx_out modified by interrupt !
	utx_in = i;
	to_tx();																			// enable TX interrupt
}
//}}}

void uputa(uint8_t* data, uint8_t length)
{
	for(uint8_t j=0; j<length; j++)
	{
		uputc(data[j]);
	}
}


/****************************** Interrupts *******************************/

//{{{
ISR(USART_RX_vect)
{
	uint8_t i = urx_in;

	ROLLOVER(i, URX_SIZE);
	if( i == urx_out )																	// buffer overflow
	{
		//URX_IEN = 0;																	// disable RX interrupt
		// Lieber einfach nur das eine Byte wegschmeißen, als gleich den Interrupt zu deaktivieren, weil das mit dem Rest Probleme gäbe
		uint8_t temp = UDR0; 															// Dadurch wird das Flag für USART_RX_vect vom System gelöscht
		temp=temp;																		// Warnung über nicht benutze Variable abstellen
		return;
	}
	urx_buff[urx_in] = UDR0;
//	sputc(UDR0);
	urx_in = i;
}
//}}}

//{{{
ISR(USART_UDRE_vect)
{
	if( utx_in == utx_out )																// nothing to sent
	{
		to_rx();
		return;
	}
	UDR0 = utx_buff[utx_out];
	ROLLOVER(utx_out, UTX_SIZE);
}
//}}}
