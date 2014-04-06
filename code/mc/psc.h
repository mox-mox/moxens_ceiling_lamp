#ifndef PSC_H
#define PSC_H

#include <avr/io.h>
#include "main.h"

#define R1 OCR0RB //PSCOUT01
#define G1 OCR1RB //PSCOUT11
#define B1 OCR0RA //PSCOUT00

#define R2 OCR1RA //PSCOUT10
#define G2 OCR2RA //PSCOUT20
#define B2 OCR2RB //PSCOUT21


void init_psc();

inline void start_psc()
{
	// Only need to start PSC0 explicitly, the others are slaved to it //PCTL0 |= (1<<PRUN0);
	SBIT(PCTL0, PRUN0) = 1;
}
inline void stop_psc()
{
	// Only need to stop PSC0 explicitly, the others are slaved to it //PCTL0 &= ~(1<<PRUN0);
	SBIT(PCTL0, PRUN0) = 0;
}







#endif /* PSC_H */
