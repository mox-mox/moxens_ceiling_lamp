#include <avr/io.h>
#include <avr/interrupt.h>

//#ifndef SIGNAL // !!!SIGNAL IS DEPRECATED!!!
//#include <avr/signal.h>
//#endif // SIGNAL

#include "suart.h"

// Folgende Zeile einkommentieren, falls FIFO verwendet werden soll
#include "fifo.h"

// Baudrate festlegen
#define BAUDRATE 38400

// "NO Operation": benoetigt einen Taktzyklus, ansonsten kein Effekt
#define nop() __asm volatile ("nop")

#ifdef SUART_TXD
// definiere Port und Pin fuer TXD
	#define SUART_TXD_PORT PORTB
	#define SUART_TXD_DDR  DDRB
	#define SUART_TXD_BIT  PB1         //OC1A
	static volatile uint16_t outframe;
#endif	// SUART_TXD

// definiere Port und Pin fuer RXD
#ifdef SUART_RXD
	#define SUART_RXD_PORT PORTB
	#define SUART_RXD_PIN  PINB
	#define SUART_RXD_DDR  DDRB
	#define SUART_RXD_BIT  PB0			// ICP1
	static volatile uint16_t inframe;
	static volatile uint8_t inbits, received;

// Variablen fuer FIFO
	#ifdef FIFO_H
		#define INBUF_SIZE 32	// must be 2^n
		//#define OUTBUF_SIZE 32	// must be 2^n
		static uint8_t inbuf[INBUF_SIZE];
		//static uint8_t outbuf[OUTBUF_SIZE];
		fifo_t infifo;
		//fifo_t outfifo;
	#else	// FIFO_H
		static volatile uint8_t indata;
	#endif	// FIFO_H
#endif	// SUART_RXD

// Initialisierung für einen ATmega168
// Für andere AVR-Derivate sieht dies vermutlich anders aus:
// Registernamen ändern sich (zB TIMSK1 anstatt TIMSK, etc).
void suart_init()
{
	DDRC |= (1 << 4);
	uint8_t tifr = 0;
	uint8_t sreg = SREG;
	cli();

	// Mode #4 für Timer1
	// und volle MCU clock
	TCCR1A = 0;
	TCCR1B =
		// OCR1A (Output Compare Register) laesst Timer ueberlaufen
	    (1 << WGM12) |
		// clock/1 (no prescaling)
	    (1 << CS10) |
		// IC on Falling Edge
	    (0 << ICES1) |
		// IC Noise Cancel
	    (1 << ICNC1);

	// OutputCompare für gewünschte Timer1 Frequenz
	OCR1A = (uint16_t) ((uint32_t) F_CPU / BAUDRATE);

#ifdef SUART_RXD
		// Port und Pin aktivieren
		SUART_RXD_DDR  &= ~(1 << SUART_RXD_BIT);
		SUART_RXD_PORT |=  (1 << SUART_RXD_BIT);
		// Input Capture Interrupt aktivieren
		TIMSK1 |= (1 << ICIE1);
		tifr  |=
			// setze Input Capture Flag, sobald timer top value (ocr) erreicht
		    (1 << ICF1) |
			// setze Output Compare B match flag, sobald der timer Wert mit dem Output Compare Register (OCR) uebereinstimmt
		    (1 << OCF1B);
#else
		// Input Capture Interrupt deaktivieren
		TIMSK1 &= ~(1 << ICIE1);
#endif		// SUART_RXD

#ifdef SUART_TXD
		// setze Output Compare A match flag, sobald der timer Wert mit dem Output Compare Register (OCR) uebereinstimmt
		tifr |= (1 << OCF1A);
		// Port und Pin aktivieren
		SUART_TXD_PORT |= (1 << SUART_TXD_BIT);
		SUART_TXD_DDR  |= (1 << SUART_TXD_BIT);
		outframe = 0;
#endif		// SUART_TXD

	TIFR1 = tifr;

	SREG = sreg;

#ifdef FIFO_H
		fifo_init(&infifo, inbuf, INBUF_SIZE);
#endif		// FIFO_H
}


#ifdef SUART_TXD
	//void suart_puts (char *s) // no use for this function, sice we use a binary protocol
	//{
	//    while (*s)
	//    {                   // so lange *s != '\0' also ungleich dem "String-Endezeichen"
	//        suart_putc(*s);
	//        s++;
	//    }
	//}

//void suart_putc (const char c)
	void suart_putc(uint8_t data)	// TODO: Use a fifo for writing
	{
		// interrupts deaktivieren
		cli();
		do
		{
			// interrupts aktivieren
			sei();
			// no operation
			nop();
			// interrupts deaktivieren
			cli();	// yield();
		} while ( outframe );

		// frame = *.P.7.6.5.4.3.2.1.0.S   S=Start(0), P=Stop(1), *=Endemarke(1)
		//outframe = (3 << 9) | (((uint16_t) c) << 1);
		outframe = (3 << 9) | (((uint16_t) data) << 1);

		// Timer Output Compare A Match interrupt aktivieren
		TIMSK1 |= (1 << OCIE1A);
		// setze Output Compare A match flag, sobald der timer Wert mit dem Output Compare Register (OCR) uebereinstimmt
		TIFR1   = (1 << OCF1A);

		// interrupts aktivieren
		sei();
	}
#endif	// SUART_TXD



#ifdef SUART_TXD
	//SIGNAL (SIG_OUTPUT_COMPARE1A) DEPRECATED
	ISR(TIMER1_COMPA_vect)
	{
		uint16_t data = outframe;

		if ( data & 1 )
		{
			// setze TXD Pin auf HIGH
			SUART_TXD_PORT |=  (1 << SUART_TXD_BIT);
		}
		else
		{
			// setze TXD Pin auf LOW
			SUART_TXD_PORT &= ~(1 << SUART_TXD_BIT);
		}

		if ( 1 == data )
		{
			// Timer Output Compare A Match interrupt deaktivieren
			TIMSK1 &= ~(1 << OCIE1A);
		}

		outframe = data >> 1;
	}
#endif	// SUART_TXD


#ifdef SUART_RXD
	//SIGNAL (SIG_INPUT_CAPTURE1) DEPRECATED
	ISR(TIMER1_CAPT_vect)
	{
		uint16_t icr1  = ICR1;
		uint16_t ocr1a = OCR1A;

		// Eine halbe Bitzeit zu ICR1 addieren (modulo OCR1A) und nach OCR1B
		uint16_t ocr1b = icr1 + ocr1a / 2;
		if ( ocr1b >= ocr1a )
		{
			ocr1b -= ocr1a;
		}
		// Output Compare Register 1B
		OCR1B = ocr1b;

		// setze Output Compare B match flag, sobald der timer Wert mit dem Output Compare Register (OCR) uebereinstimmt
		TIFR1 = (1 << OCF1B);
		// Input Capture Interrupt Flag entfernen und aktiviere Output Compare B Match interrupt
		TIMSK1 = (TIMSK1 & ~(1 << ICIE1)) | (1 << OCIE1B);
		inframe = 0;
		inbits = 0;
	}
#endif	// SUART_RXD


#ifdef SUART_RXD
	//SIGNAL (SIG_OUTPUT_COMPARE1B) DEPRECATED
	ISR(TIMER1_COMPB_vect)
	{
		uint16_t data = inframe >> 1;

		// falls ein Bit empfangen wird
		if ( SUART_RXD_PIN & (1 << SUART_RXD_BIT))
		{
			data |= (1 << 9);
		}

		uint8_t bits = inbits + 1;

		if ( 10 == bits )
		{
			if ((data & 1) == 0 )
				if ( data >= (1 << 9))
				{
	#ifdef FIFO_H
						if(fifo_push(&infifo, data >> 1))
						{
							//TODO: CTS auf 1 setzen, somit wird der Datenstrom angehalten
						}
	#else
						indata = data >> 1;
	#endif		// FIFO_H
					received = 1;
				}

			// Input Capture Interrupt Flag setzen und deaktiviere Output Compare B Match interrupt
			TIMSK1 = (TIMSK1 & ~(1 << OCIE1B)) | (1 << ICIE1);
			// setze Input Capture Flag, sobald timer top value (ocr) erreicht
			TIFR1 = (1 << ICF1);
		}
		else
		{
			inbits = bits;
			inframe = data;
		}
	}
#endif	// SUART_RXD


// ## #ifdef SUART_RXD
// ## 	#ifdef FIFO_H
// ## 
// ## //uint8_t suart_getc_wait()
// ## //{
// ## //	uint8_t data;
// ## //    fifo_get_wait(&infifo, &data);
// ## //	return data;
// ## //}
// ## 
// ## 		uint8_t suart_getc()
// ## 		{
// ## 			uint8_t data;
// ## 			fifo_get(&infifo, &data);
// ## 			return data;
// ## 		}
// ## 
// ## 	#else	// FIFO_H
// ## 
// ## 		uint8_t suart_getc_wait()
// ## 		{
// ## 			while ( !received ) ;	//   {}
// ## 			received = 0;
// ## 
// ## 			return (int) indata;
// ## 		}
// ## 
// ## 		uint8_t suart_getc_nowait()
// ## 		{
// ## 			if ( received )
// ## 			{
// ## 				received = 0;
// ## 				return (int) indata;
// ## 			}
// ## 
// ## 			return -1;
// ## 		}
// ## 	#endif	// FIFO_H
// ## #endif	// SUART_RXD
