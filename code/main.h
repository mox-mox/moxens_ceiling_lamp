#ifndef MAIN_H
#define MAIN_H

//#include <avr/io.h>
//#include <inttypes.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//#include "uart.h"
//#include "suart.h"
//#include "dynaservo.h"
//#include "gaits.h"
//#include "terminal.h"
//#include "snake.h"
//#include "timer.h"

//
//
////{{{
//#pragma GCC diagnostic ignored "-Wpedantic"  /* no diagnostic for this one */
//// Access bits like variables:
//	struct bits
//	{
//		uint8_t b0 : 1;
//		uint8_t b1 : 1;
//		uint8_t b2 : 1;
//		uint8_t b3 : 1;
//		uint8_t b4 : 1;
//		uint8_t b5 : 1;
//		uint8_t b6 : 1;
//		uint8_t b7 : 1;
//	} __attribute__((__packed__));
//#pragma GCC diagnostic pop
//
//#define SBIT_(port, pin) ((*(volatile struct bits*) &port).b ## pin)
//#define SBIT(x, y)   SBIT_(x, y)
////}}}
//
//
//
//
//#define SUCCESS 0
//#define FAILURE 1
//
//// Ein paar Zustandsvariablen, die bestimmen, wie sich die Schlange verhält
//uint8_t status_output_level;
//uint8_t current_gait;
//uint8_t status_flag;
//float timestep;
//
//
////uint8_t test_byte;
//volatile uint8_t gait_next_step_flag;															// Den nächsten Zeitschritt des Gaits berechnen
//volatile uint8_t status_packet_flag;
//volatile uint16_t stiffnessOff_flag;

#endif
