/************************************************************************/
/*                                                                      */
/*                      Software UART with FIFO, using T1               */
/*                                                                      */
/*              Author: P. Dannegger, modified: Mox                     */
/*                                                                      */
/*              http://www.mikrocontroller.net/topic/125651             */
/*                                                                      */
/************************************************************************/

#ifndef SUART_H
#define SUART_H

#include <stdint.h>

// Ressources used: Timer1, PB0, PB1

// size must be in range 2 .. 256
#define STX_SIZE	20u
#define SRX_SIZE	255u // _MUST_ be a multiple of 2

//#define SUART_BAUDRATE	38400u
//#define SUART_BAUDRATE	9600u
#define SUART_BAUDRATE	38400u
//#define SUART_BAUDRATE	57600u
//#define SUART_BAUDRATE	19200u

//{{{


#define	STXD		SBIT( PORTB, PB1 )		// = OC1A
#define	STXD_DDR	SBIT( DDRB,  PB1 )

#define	SRXD_PIN	SBIT( PINB,  PB0 )		// = ICP
//}}}

extern volatile uint16_t srx_wait_flag;


void suartInit(void);
void sputc(uint8_t c);						// send byte
void sputa(uint8_t *a, uint8_t length);		// send whole Array



//{{{ Unsused

//void uputla(uint8_t *a, uint16_t length)		// send even longer Array

// All the get stuff cannot be used, because we use another fifo
uint8_t sgetc_with_timeout(uint8_t* received_byte);
uint8_t skbhit( void );						// check for incoming data
//
//#define	sputs(x)	uputs_((uint8_t*)(x))	// avoid char warning
//void sputs_( uint8_t *s );					// send string from SRAM
//}}}

#endif /* SUART */
