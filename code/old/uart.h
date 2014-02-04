/************************************************************************/
/*                                                                      */
/*                      Interrupt driven buffered UART                  */
/*                                                                      */
/*              Author: P. Dannegger, modified: Mox                     */
/*                                                                      */
/*              !Dieser Code wurde teils massiv verändert!              */
/*              Falls man ihn also für andere Projekte                  */
/*              nutzen möchte, sollte man das Original als              */
/*              Ausgangsbasis nutzen, da diese Version auf              */
/*              einen Spezialfall angepasst ist, nämlich,               */
/*              dass nur die Sende, oder Empfangseinheit                */
/*              aktiv sein darf (wegen 1-Draht-Bus)                     */
/*                                                                      */
/*              http://www.mikrocontroller.net/topic/125651             */
/*                                                                      */
/************************************************************************/

#ifndef UART_H
#define UART_H
#include "stdint.h"

// Ressources used here: UART, PD0, PD1

// Wird in der main.c verwendet, muss aber nicht verwendet werden!!
//#define UART_BAUDRATE 9600UL							// Baudrate
#define UART_BAUDRATE 19200UL							// Baudrate
//#define UART_BAUDRATE 115200UL						// Baudrate
//#define UART_BAUDRATE 200000UL						// Baudrate
//#define UART_BAUDRATE 1000000UL						// Baudrate

// Baud Berechnungen
#define UBRR_VAL ((F_CPU+UART_BAUDRATE*8)/(UART_BAUDRATE*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/UART_BAUDRATE) // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error "Systematischer Fehler der Baudrate groesser 1% und damit zu hoch!"
#endif


// size must be in range 2 .. 256
#define URX_SIZE	64									// usable: URX_SIZE + 2 (4 .. 258)
#define UTX_SIZE	64									// usable: UTX_SIZE + 1 (3 .. 257)


void uartInit();

uint8_t ukbhit0();										// 0 <=> rx buffer empty
uint8_t ugetc();										// get received byte
uint8_t ugetc_with_timeout(uint8_t* received_byte);
//uint8_t utx_ready( void );							// 0 <=> tx still busy
void uputc(uint8_t c );									// send byte
void uputa(uint8_t* data, uint8_t length);

//void uputs_(uint8_t *s );								// send string from SRAM
//#define	uputs(x)	uputs_((uint8_t*)(x))			// avoid char warning


#define	UTX_IEN	SBIT( UCSR0B, UDRIE0 )
#define	URX_IEN	SBIT( UCSR0B, RXCIE0 )

uint8_t ukbhit(void);

void to_rx();
void to_tx();


volatile uint16_t rx_wait_flag;



//uint8_t uart_tx_wait_flag;

uint8_t utx_buffer_filled();

#endif /* UART_H */









