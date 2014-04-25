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
//0xFF      0xFF      0xAA    0xBB    0xCC    0xDD    AA+BB+CC+DD-1

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




// Geschwindigkeitsabschätzung des Protokolls
//
// Keine Parität, 1 Stopbit
//*octave:5> bitrate=4800, total_byte_length=8+2, command_length=7, byterate=bitrate/total_byte_length, commandrate=byterate/command_length
//bitrate =  4800
//total_byte_length =  10
//command_length =  7
//byterate =  480
//commandrate =  68.571
//
//
// Mit Parität, 2 Stopbits
//*octave:6> bitrate=4800, total_byte_length=8+4, command_length=7, byterate=bitrate/total_byte_length, commandrate=byterate/command_length
//bitrate =  4800
//total_byte_length =  12
//command_length =  7
//byterate =  400
//commandrate =  57.143
