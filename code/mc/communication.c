#include "communication.h"
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "humidity.h"
#include "colours.h"


inline void handle_command(instruction command, uint8_t data);

void init_communication(uint32_t baudrate)
{
	const uint16_t bauddivider = ((uint16_t) ((F_CPU / baudrate / 16)-0.5));		// Calculate settings for the baud rate...
	UBRR = bauddivider;																// ...and set the baud rate

	UCSRA = (0<<U2X) | (0<<MPCM);													// no U2X, no MPCM
	UCSRB = (1<<RXCIE) | (0<<TXCIE), (1<<UDRIE), (1<<RXEN), (1<<TXEN), (0<<UCSZ2);	// Receive Interrupt, Data Empty Interrupt RX and TX set
	UCSRC = (0<<UMSEL0) | (1<<UPM1) | (1<<UPM0) | (1<<USBS) | (1<<UCSZ1) | (1<<UCSZ2) | (0<<UCPOL);	// Async, odd parity, 2 stop bits, 8-bit frames
}


ISR(USART_RX_vect)
{
	static uint8_t state=0;
	static uint8_t command;
	static uint8_t data;
	static uint8_t received_byte;

	// Read Error codes and put the byte into the buffer
	if(UCSRA & ((1<<FE) | (1<<DOR) | (1<<UPE)))					// If there is a frame- or parity- or overrun- error...
	{
		state=0;												// ...drop this packet
		received_byte=UDR;										// read this to avoid a data overrun error from the next byte
		return;
	}
	received_byte=UDR;
	switch(state)
	{
		case 0:													// 1st start bit
		case 1:													// 2nd start bit
			if(received_byte == STARTBYTE) state++;
			else state=0;
			break;
		case 2:													// Command
			command=received_byte;
			state++;
			break;
		case 3:
			data=received_byte;
			state++;
			break;
		case 4:													// Checksum
			state=0;
			if(command+data == received_byte)	// valid checksum
				handle_command(command, data);
		default:		// WTF happened?
			state=0;
			break;
	}
}


#define TX_FIFO_SIZE 16 // Must be a power of 2
volatile uint8_t commands_to_send[TX_FIFO_SIZE][2];
volatile uint8_t tx_write_index;
volatile uint8_t tx_read_index;
volatile uint8_t tx_fifo_crash_flag = 0;
void send_command(instruction command, uint8_t data)
{
	uint8_t sreg=SREG;
	cli();
	tx_write_index++;
	tx_write_index &= (TX_FIFO_SIZE-1);							// increment with overflow
	//if(tx_write_index==tx_read_index) tx_fifo_crash_flag=1;	//TODO: Is there an off-by-one-error?
	commands_to_send[tx_write_index][0]=(uint8_t) command;
	commands_to_send[tx_write_index][1]=data;
	SREG=sreg;
}






ISR(USART_UDRE_vect)
{
	static volatile uint8_t state=0;
	static volatile uint8_t command;
	static volatile uint8_t data;

	switch(state)
	{
		case 0:
			// abuse command to hold a temporary variable to avoid creating a stack frame
			command = (tx_read_index+1) & (TX_FIFO_SIZE-1);
			if(command == tx_write_index)						// If the buffer is empty... // TODO: Is there an off-by-one-error?
			{
				SBIT( UCSRB, UDRIE) = 0;						// ...disable this interrupt...
				return;
			}
			tx_read_index = command;						// command contains the incremented address //TODO: Should the adress be incremented after reading?
			command = commands_to_send[tx_read_index][0];	// All hail the
			data    = commands_to_send[tx_read_index][1];	// double buffer!!
			// note the case fall through...
		case 1:
			UDR=STARTBYTE;
			state++;
			break;
		case 2:
			UDR=command;
			state++;
			break;
		case 3:
			UDR=data;
			state++;
			break;
		case 4:
			UDR=command+data;
			state=0;
			break;
		default:		// WTF happened?
			state=0;
			break;
	}
}






inline void handle_command(instruction command, uint8_t data)
{
	sei();							// Handle commands while allowing interrupts
	switch(command)
	{
		//{{{
		case humidity1:
			measure_humidity();
			break;
		case humidity2:
			//send_command(humidity1, (uint8_t) last_humidity_measurement>>8);
			//send_command(humidity2, (uint8_t) last_humidity_measurement);
			break;
		//}}}

		//{{{
		case on_off:
			if(!data)				// Data == 0 means "Lights off NOW!"
			{
				stop_psc();			// Turn the light off completely
			}
			else					// Data > 0 asks for the lights to be turned back on
			{
				start_psc();		// Turn the lights on
				if(data>1)			// Data > 1 requests full brightness
				{
					R1=0x0FFF;		// Set
					G1=0x0FFF;		// all
					B1=0x0FFF;		// channels
					R2=0x0FFF;		// to
					G2=0x0FFF;		// maximum
					B2=0x0FFF;		// brightness
				}
			}
			break;
		//}}}

		//{{{
		case set_r:
			new_colour1.r=data;
			new_colour2.r=data;
			break;
		case set_g:
			new_colour1.g=data;
			new_colour2.g=data;
			break;
		case set_b:
			new_colour1.b=data;
			new_colour2.b=data;
			break;
		//}}}

		//{{{
		case set_r1:
			new_colour1.r=data;
			break;
		case set_g1:
			new_colour1.g=data;
			break;
		case set_b1:
			new_colour1.b=data;
			break;
		case set_r2:
			new_colour2.r=data;
			break;
		case set_g2:
			new_colour2.g=data;
			break;
		case set_b2:
			new_colour2.b=data;
			break;
		//}}}

		//{{{
		case update_colour:
			update_colour1_values();
			update_colour2_values();
			break;
		case update_colour1:
			update_colour1_values();
			break;
		case update_colour2:
			update_colour2_values();
			break;
		//}}}

		//{{{
		case get_r1:
			send_command(set_r1, get_r1_value());
			break;
		case get_g1:
			send_command(set_g1, get_g1_value());
			break;
		case get_b1:
			send_command(set_b1, get_b1_value());
			break;
		case get_r2:
			send_command(set_r2, get_r2_value());
			break;
		case get_g2:
			send_command(set_g2, get_g2_value());
			break;
		case get_b2:
			send_command(set_b2, get_b2_value());
			break;
		//}}}
		default:
			break;
	}
}
