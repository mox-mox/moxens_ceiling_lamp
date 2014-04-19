#ifndef COLOURS_H
#define COLOURS_H

#include "psc.h"
#include <avr/io.h>

//#define R1 OCR0RB //PSCOUT01
//#define G1 OCR1RB //PSCOUT11
//#define B1 OCR0RA //PSCOUT00
//
//#define R2 OCR1RA //PSCOUT10
//#define G2 OCR2RA //PSCOUT20
//#define B2 OCR2RB //PSCOUT21

// Multiplication by 16 is just a left shift by 4 which converts the 8-bit ligtness value into the 12-bit value for the pwm.
// So to scale down one colour, decrease its scale.
#define R_SCALE 16
#define G_SCALE 16
#define B_SCALE 16

void init_colours();

inline void set_rgb1_values(const uint8_t colour[3])
{
	B1=colour[2]*B_SCALE;	// The chronological order is important because autolocking ensures
	R1=colour[0]*R_SCALE;	// the update of the PSC internal registers will only be done at the end
	G1=colour[1]*G_SCALE;	// of the PSC cycle if the Output Compare Register RB has been the last written
}

inline void set_rgb2_values(const uint8_t colour[3])
{
	R2=colour[0]*R_SCALE;	// The chronological order is important because autolocking ensures
	G2=colour[1]*G_SCALE;	// the update of the PSC internal registers will only be done at the end
	B2=colour[2]*B_SCALE;	// of the PSC cycle if the Output Compare Register RB has been the last written
	G1=G1;	// Enable update of R2
}

//{{{
inline void get_rgb1_value2(uint8_t colour[3])
{
	colour[0]=R1/R_SCALE;
	colour[1]=G1/G_SCALE;
	colour[2]=B1/B_SCALE;
}
inline void get_rgb2_value2(uint8_t colour[3])
{
	colour[0]=R2/R_SCALE;
	colour[1]=G2/G_SCALE;
	colour[2]=B2/B_SCALE;
}
//}}}


#endif /* COLOURS_H */
