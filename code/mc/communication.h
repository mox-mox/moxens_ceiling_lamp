#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#define BAUD		9600

void init_communication(uint32_t baudrate);
void send_command(uint8_t command, uint8_t data);









//Protocol:

//Startbyte Startbyte Command Data Checksum
//0xFF      0xFF      0xYY    0xZZ XX+YY+ZZ

#define STARTBYTE 0xFF		// This is not to be changed, but to allow 

//{{{
enum instructions_enum
{	// Instructions			// Arguments
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

	invalid_command=STARTBYTE;	// With this, detecting the startbytes becomes mostly unambigious.
								// There is still the possibility, that the checksum becomes STARTBYTE, but this should not be that much of a problem.
} __attribute__((packed));
typedef instructions instructions_enum;
//}}}

#endif /* COMMUNICATION_H */
