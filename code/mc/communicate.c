#include "communicate.h"



void uartInit(uint32_t baudrate)
{
	const uint16_t bauddivider = ((uint16_t)((F_CPU / baudrate / 16)-0.5));			// Calculate settings for the baud rate...
	UBRR = bauddivider;																// ...and set the baud rate

	UCSRA = (0<<U2X) | (0<<MPCM);													// no U2X, no MPCM
	UCSRB = (1<<RXCIE) | (0<<TXCIE), (1<<UDRIE), (1<<RXEN), (1<<TXEN), (0<<UCSZ2);	// Receive Interrupt, Data Empty Interrupt RX and TX set
	UCSRC = (0<<UMSEL) | (1<<UPM1) | (1<<UPM0) | (1<<USBS) | (1<<UCSZ1) | (1<<UCSZ2) | (0<<UCPOL); // Async, odd parity, 2 stop bits, 8-bit frames
}

uint8_t received_commands[16][3];
uint8_t received_commend_index;

uint8_t rx_buf[8];
uint8_t rx_buf_index;

ISR(USART_RX_vect)
{
	// Read Error codes and put the byte into the buffer
	if(UCSRA & ((1<<FE) | (1<<UPE)))	// Frame or Parity error
	{
		rx_buf[0] 
	}
	if(UCSRA & (1<<DOR))				// Data Overrun
	{
		rx_buf[1] = UCSRA;
	}
	rx_buf[rx_buf_index++] = UDR;

	if(rx_buf_index==6)					// one full command is waiting in the buffer
	{
		if(rx_buf[0]




	}

}

uint8_t commands_to send[16][3];

ISR(USART_UDRE_vect)
{
}









void handle_commands(uint8_t ticket, unit8_t command, uint8_t data)
{

}
