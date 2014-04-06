#include "communicate.h"



void uartInit(uint32_t baudrate)
{
	const uint16_t bauddivider = ((uint16_t) ((F_CPU / baudrate / 16)-0.5));		// Calculate settings for the baud rate...
	UBRR = bauddivider;																// ...and set the baud rate

	UCSRA = (0<<U2X) | (0<<MPCM);													// no U2X, no MPCM
	UCSRB = (1<<RXCIE) | (0<<TXCIE), (1<<UDRIE), (1<<RXEN), (1<<TXEN), (0<<UCSZ2);	// Receive Interrupt, Data Empty Interrupt RX and TX set
	UCSRC = (0<<UMSEL) | (1<<UPM1) | (1<<UPM0) | (1<<USBS) | (1<<UCSZ1) | (1<<UCSZ2) | (0<<UCPOL);	// Async, odd parity, 2 stop bits, 8-bit frames
}


ISR(USART_RX_vect)
{
	static uint8_t rx_state=0;
	static uint8_t rx_ticket;
	static uint8_t rx_command;
	static uint8_t rx_data;
	static uint8_t received_byte;

	// Read Error codes and put the byte into the buffer
	if(UCSRA & ((1<<FE) | (1<<DOR) | (1<<UPE)))					// Frame- or Parity- or Overrun- error
	{
		rx_state=0;
		received_byte=UDR;										// read this to avoid a data overrun error from the next byte
		return;
	}
	received_byte=UDR;
	switch(rx_state)
	{
		case 0:													// 1st start bit
		case 1:													// 2nd start bit
			if(received_byte == STARTBYTE) state++;
			else rx_state=0;
			break;
		case 2:													// Ticket
			rx_ticket=received_byte;
			rx_state++;
			break;
		case 3:													// Command
			rx_command=received_byte;
			rx_state++;
			break;
		case 4:
			rx_data=received_byte;
			rx_state++;
			break;
		case 5:													// Checksum
			rx_state=0;
			if(rx_ticket+rx_command+rx_data == received_byte)	// valid checksum
			{
				send_command(ticket, ack, 0);
				handle_command(rx_ticket, rx_command, rx_data);
			}
			else
			{
				send_command(ticket, nak, 0);
			}
		default:		// WTF happened?
			state=0;
			break;
	}
}


#define TX_FIFO_SIZE 32
volatile uint8_t commands_to send[TX_FIFO_SIZE][3];
volatile uint8_t tx_write_index;
volatile uint8_t tx_read_index;
volatile uint8_t tx_fifo_crash_flag = 0;
void send_command(uint8_t ticket, instruction command, uint8_t data)
{
	uint8_t sreg=SREG;
	cli();
	++tx_write_index &= TX_FIFO_SIZE-1;							// increment with overflow
	//if(tx_write_index==tx_read_index) tx_fifo_crash_flag=1;	//TODO: Is there an off-by-one-error?
	commands_to_send[tx_write_index][0]=ticket;
	commands_to_send[tx_write_index][1]=(uint8_t) command;
	commands_to_send[tx_write_index][2]=data;
	SREG=sreg;
}






ISR(USART_UDRE_vect)
{
	static volatile uint8_t state=0;
	static volatile uint8_t ticket;
	static volatile uint8_t command;
	static volatile uint8_t data;

	switch(state)
	{
		case 0:
			// abuse ticket to hold a temporary variable to avoid creating a stack frame
			ticket = (tx_read_index+1) & TX_FIFO_SIZE-1;
			if(ticket == tx_write_index)						// If the buffer is empty... // TODO: Is there an off-by-one-error?
			{
				SBIT( UCSRB, UDRIE) = 0;						// ...disable this interrupt...
				return;
			}
			tx_read_index = ticket;						// Ticket contains the incremented address //TODO: Should the adress be incremented after reading?
			ticket  = command_to_send[tx_read_index][0];	// All hail
			command = command_to_send[tx_read_index][1];	// the double
			data    = command_to_send[tx_read_index][2];	// buffer!!
			// note the case fall through...
		case 1:
			UDR=STARTBYTE;
			state++;
			break;
		case 2:
			UDR=ticket;
			state++;
			break;
		case 3:
			UDR=command;
			state++;
			break;
		case 4:
			UDR=data;
			state++;
			break;
		case 5:
			UDR=ticket+command+data;
			state=0;
			break;
		default:		// WTF happened?
			state=0;
			break;
	}




}









inline void handle_commands(uint8_t ticket, instruction command, uint8_t data)
{
	sei();							// Handle commands while allowing interrupts
	switch(command)
	{
		//{{{
		//case ack:					// should not be received
		//case nak:
		//	nop();
		//	break;
		//}}}

		//{{{
		case humidity1:
		case humidity2:
			measure_humidity();
			break;
		//}}}

		//{{{
		case on_off:
			if(!data)				// Data == 0 means "Lights off NOW!"
			{
				stop_pwm();			// Turn the light off completely
			}
			else					// Data > 0 asks for the lights to be turned back on
			{
				start_pwm();		// Turn the lights on
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
			update_colour1();
			break;
		case update_colour1:
			update_colour1();
			break;
		case update_colour2:
			update_colour2();
			break;
		//}}}

		//{{{
		case get_r1:
			send_command(ticket, set_r1, get_r1());
			break;
		case get_g1:
			send_command(ticket, set_g1, get_g1());
			break;
		case get_b1:
			send_command(ticket, set_b1, get_b1());
			break;
		case get_r2:
			send_command(ticket, set_r2, get_r2());
			break;
		case get_g2:
			send_command(ticket, set_g2, get_g2());
			break;
		case get_b2:
			send_command(ticket, set_b2, get_b2());
			break;
		//}}}
		default:
			break;
	}
}
