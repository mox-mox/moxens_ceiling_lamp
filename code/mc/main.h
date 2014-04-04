#ifndef MAIN_H
#define MAIN_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
//#include <inttypes.h>
#include <util/delay.h>







//			Easier type writing:

//typedef uint8_t		u8;
//typedef int8_t		s8;
//typedef uint16_t	u16;
//typedef int16_t		s16;
//typedef uint32_t	u32;
//typedef int32_t		s32;


// The following part of this file is taken from Peter Dannegger: http://www.mikrocontroller.net/topic/101472

//{{{
#pragma GCC diagnostic ignored "-Wpedantic"  /* no diagnostic for this one */
// Access bits like variables:
	struct bits
	{
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	} __attribute__((__packed__));
#pragma GCC diagnostic pop

//}}}

#define SBIT_(port,pin) ((*(volatile struct bits*)&port).b##pin)
#define	SBIT(x,y)	SBIT_(x,y)


//			Optimization improvements

// always inline function x:
#define AIL(x)   static x __attribute__ ((always_inline)); static x


// never inline function x:
#define NIL(x)   x __attribute__ ((noinline)); x


// volatile access (reject unwanted removing access):
#define vu8(x)  (*(volatile u8*)&(x))
#define vs8(x)  (*(volatile s8*)&(x))
#define vu16(x) (*(volatile u16*)&(x))
#define vs16(x) (*(volatile s16*)&(x))
#define vu32(x) (*(volatile u32*)&(x))
#define vs32(x) (*(volatile s32*)&(x))






#define SUCCESS 0
#define FAILURE 1


#endif









