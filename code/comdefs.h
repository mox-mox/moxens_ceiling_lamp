#ifndef COMDEFS_H
#define COMDEFS_H

// The Baudrate to use for communication
//#define BAUD		9600UL
#define BAUD		4800UL
#define enumBAUD	BAUD_4800

// Further settings for the serial connection:
// Asyncronous communication, odd parity, 2 stop bits, 8-bit frames

//Protocol:

//Startbyte Startbyte Command Data[0] Data[1] Data[2] Checksum
//0xFF      0xFF      0xAA    0xBB    0xCC    0xDD    AA+BB+CC+DD

#define STARTBYTE 0xFF		// This is not to be changed.

//{{{
enum instructions_enum
{	// Instructions			// Arguments
	ack,
	nak,
	humidity,				// PC->MC: NONE,             MC->PC: Result of the Measurement

	on_off,					// 0: Turn the Lights off completely, 1: Turn the lights on immediately, 2: Turn the lights on and to full brightness immediately

	set_rgb,				// New global rgb values
	set_rgb1,				// New rgb values for left half of the lamp
	set_rgb2,				// New rgb values for right half of the lamp

	get_rgb1,				// NONE
	get_rgb2,				// NONE

	// much free space for additional commands ;)

	invalid_command=STARTBYTE	// With this, detecting the startbytes becomes mostly unambigious.
								// There is still the possibility, that the checksum becomes STARTBYTE, but this should not be that much of a problem.
} __attribute__((packed));
typedef enum instructions_enum instruction;
//}}}

#endif /*COMDEFS_H*/
