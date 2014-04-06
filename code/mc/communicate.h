#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#define BAUD		9600

void send_command(uint8_t ticket, uint8_t command, uint8_t data);






#endif /* COMMUNICATE_H */



//Protocol:

//Startbit Startbit Ticket Command Data Checksum
//0xFF     0xFF     0xXX   0xYY    0xZZ XX+YY+ZZ

//{{{
enum commands				// Arguments
{
	ack,					// NONE
	nak,					// NONE

	humidity1,				// PC->MC: NONE,             MC->PC: High-Byte of the Measurement
	humidity2,				// PC->MC: Will not be sent, MC-PC: Low-Byte of the Measurement

	set_h,					// New global hue value (for both halves)
	set_s,					// New global saturation value (for both halves)
	set_v,					// New global brightness value (for both halves)
	update_hsv;				// NONE: Use the new global hsv values

	set_r,					// New global red value (for both halves)
	set_g,					// New global green value (for both halves)
	set_b,					// New global blue value (for both halves)
	update_rgb;				// NONE: Use the new global hsv values

	on_off,					// 0: Turn the Lights off immediately, 1: Turn the lights on immediately

	get_h1,					// NONE
	get_s1,					// NONE
	get_v1,					// NONE
	get_h2,					// NONE
	get_s2,					// NONE
	get_v2,					// NONE

	get_r1,					// NONE
	get_g1,					// NONE
	get_b1,					// NONE
	get_r2,					// NONE
	get_g2,					// NONE
	get_b2,					// NONE

	set_h1,					// New hue value
	set_s1,					// New saturation value
	set_v1,					// New brightness value
	update_hsv1;			// NONE
	set_h2,					// New hue value
	set_s2,					// New saturation value
	set_v2,					// New brightness value
	update_hsv2;			// NONE

	set_r1,					// New red value
	set_g1,					// New green value
	set_b1,					// New blue value
	update_rgb2;			// NONE
	set_r2,					// New red value
	set_g2,					// New green value
	set_b2,					// New blue value
	update_rgb2;			// NONE
	// much free space for additional commands ;)
} __attribute__((packed));
//}}}

#endif /* COMMUNICATE_H */
