#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#define BAUD		9600

void send_command(uint8_t ticket, uint8_t command, uint8_t data);









//Protocol:

//Startbyte Startbyte Ticket Command Data Checksum
//0xFF      0xFF      0xXX   0xYY    0xZZ XX+YY+ZZ

#define STARTBYTE 0xFF

//{{{
enum instructions			// Arguments
{
	ack,					// NONE
	nak,					// NONE

	humidity1,				// PC->MC: NONE,             MC->PC: High-Byte of the Measurement
	humidity2,				// PC->MC: Will not be sent, MC-PC: Low-Byte of the Measurement

	on_off,					// 0: Turn the Lights off completely, 1: Turn the lights on immediately, 2: Turn the lights on and to full brightness immediately

	set_r,					// New global red value (for both halves)
	set_g,					// New global green value (for both halves)
	set_b,					// New global blue value (for both halves)

	set_r1,					// New red value
	set_g1,					// New green value
	set_b1,					// New blue value
	set_r2,					// New red value
	set_g2,					// New green value
	set_b2,					// New blue value

	update_colour;			// NONE: Use the new global hsv values
	update_rgb1;			// NONE
	update_rgb2;			// NONE

	get_r1,					// NONE
	get_g1,					// NONE
	get_b1,					// NONE
	get_r2,					// NONE
	get_g2,					// NONE
	get_b2,					// NONE

	// much free space for additional commands ;)

	invalid_command=255;	// With this, detecting 0xFF 0xFF as startbyte becomes unambigious
} __attribute__((packed));
//}}}

#endif /* COMMUNICATE_H */
