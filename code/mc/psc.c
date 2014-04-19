#include "psc.h"
#include "main.h"
#include <avr/interrupt.h>

void init_psc()
{
	// Don't care for ADC-sync, Enable PSCn1 and PSCn0 outputs, disable PSC22 and PSC23 outputs
	PSOC0=                           (0<<PSYNC01)  | (0<<PSYNC00)  |               (1<<POEN0B) |               (1<<POEN0A);
	PSOC1=                           (0<<PSYNC1_1) | (0<<PSYNC1_0) |               (1<<POEN1B) |               (1<<POEN1A);
	PSOC2= (0<<POS23) | (0<<POS22) | (0<<PSYNC2_1) | (0<<PSYNC2_0) | (0<<POEN2D) | (1<<POEN2B) | (0<<POEN2C) | (1<<POEN2A);

	// Initialise the set- and reset register to 0
	OCR0SA=0;
	OCR0RA=0;
	OCR0SB=0;
	OCR0RB=0;

	OCR1SA=0;
	OCR1RA=0;
	OCR1SB=0;
	OCR1RB=0;

	OCR2SA=0;
	OCR2RA=0;
	OCR2SB=0;
	OCR2RB=0;

	// no 50% mode, autolock, no lock bit, one Ramp mode, active low polarity, slow clock input, don't use the output matrix

	////  Bit 6 - PALOCKn: PSC n Autolock
	////  When this bit is set, the Output Compare Registers RA, SA, SB, the Output Matrix POM2 and
	////  the PSC Output Configuration PSOCn can be written without disturbing the PSC cycles. The
	////  update of the PSC internal registers will be done at the end of the PSC cycle if the Output Com-
	////  pare Register RB has been the last written.

	PCNF0= (0<<PFIFTY0) | (1<<PALOCK0) | (0<<PLOCK0) | (0<<PMODE01) | (0<<PMODE00) | (0<<POP0) | (0<<PCLKSEL0)             ;
	PCNF1= (0<<PFIFTY2) | (1<<PALOCK1) | (0<<PLOCK1) | (0<<PMODE11) | (0<<PMODE10) | (0<<POP1) | (0<<PCLKSEL1)             ;
	PCNF2= (0<<PFIFTY2) | (1<<PALOCK2) | (0<<PLOCK2) | (0<<PMODE21) | (0<<PMODE20) | (0<<POP2) | (0<<PCLKSEL2) | (0<<POME2);

	// Don't divide the input clock:
	// Assume F_CPU = 2MHz = F_PSC => Output Frequency = F_PSC/(2^12-1) = 488.40Hz < 500Hz but still very high, so the ripple is small

	// PPRE01 | PPRE00 | Description PWM2B/3B
	// -------+ -------+-----------------------------------
	// 0      | 0      | No divider on PSC input clock
	// 0      | 1      | Divide the PSC input clock by 4
	// 1      | 0      | Divide the PSC input clock by 32
	// 1      | 1      | Divide the PSC clock by 256

	// no clock division, don't care for Flank width modulation, no async output control, PSC0 is master, the other PSCs are sync'ed to it, don't complete the cycle when turning of
	PCTL0= (0<<PPRE01) | (0<<PPRE00) | (0<<PBFM0) | (0<<PAOC0B) | (0<<PAOC0A) | (0<<PARUN0) | (0<<PCCYC0) | (0<<PRUN0);
	PCTL1= (0<<PPRE11) | (0<<PPRE10) | (0<<PBFM1) | (0<<PAOC1B) | (0<<PAOC1A) | (1<<PARUN1) | (0<<PCCYC1) | (0<<PRUN1);
	PCTL2= (0<<PPRE21) | (0<<PPRE20) | (0<<PBFM2) | (0<<PAOC2B) | (0<<PAOC2A) | (1<<PARUN2) | (0<<PCCYC2) | (0<<PRUN2);

	// Don't enable capture input, don't care for input select or edge level or filtering, ignore PSC-input
	PFRC0A= (0<<PCAE0A) | (0<<PISEL0A) | (0<<PELEV0A) | (0<<PFLTE0A) | (0<<PRFM0A3) | (0<<PRFM0A2) | (0<<PRFM0A1) | (0<<PRFM0A0);
	PFRC1A= (0<<PCAE1A) | (0<<PISEL1A) | (0<<PELEV1A) | (0<<PFLTE1A) | (0<<PRFM1A3) | (0<<PRFM1A2) | (0<<PRFM1A1) | (0<<PRFM1A0);
	PFRC2A= (0<<PCAE2A) | (0<<PISEL2A) | (0<<PELEV2A) | (0<<PFLTE2A) | (0<<PRFM2A3) | (0<<PRFM2A2) | (0<<PRFM2A1) | (0<<PRFM2A0);
	PFRC0B= (0<<PCAE0B) | (0<<PISEL0B) | (0<<PELEV0B) | (0<<PFLTE0B) | (0<<PRFM0B3) | (0<<PRFM0B2) | (0<<PRFM0B1) | (0<<PRFM0B0);
	PFRC1B= (0<<PCAE1B) | (0<<PISEL1B) | (0<<PELEV1B) | (0<<PFLTE1B) | (0<<PRFM1B3) | (0<<PRFM1B2) | (0<<PRFM1B1) | (0<<PRFM1B0);
	PFRC2B= (0<<PCAE2B) | (0<<PISEL2B) | (0<<PELEV2B) | (0<<PFLTE2B) | (0<<PRFM2B3) | (0<<PRFM2B2) | (0<<PRFM2B1) | (0<<PRFM2B0);

	// I don't use input capture, so don't trigger a capture
	PICR0= (0<<PCST0);
	PICR1= (0<<PCST1);
	PICR2= (0<<PCST2);

	// The PSC 2 Output Matrix is not used, so don't care for any settings here
	POM2= (0<<POMV2B3) | (0<<POMV2B2) | (0<<POMV2B1) | (0<<POMV2B0) | (0<<POMV2A3) | (0<<POMV2A2) | (0<<POMV2A1) | (0<<POMV2A0);

	// Disable all interrupts for the PSCs
	PIM0= (0<<PSEIE0) | (0<<PEVE0B) | (0<<PEVE0A) | (0<<PEOPE0);
	PIM1= (0<<PSEIE1) | (0<<PEVE1B) | (0<<PEVE1A) | (0<<PEOPE1);
	PIM2= (0<<PSEIE2) | (0<<PEVE2B) | (0<<PEVE2A) | (0<<PEOPE2);

	// No interrupt flags should be set, even though they are disabled
	PIFR0 = (1<<POAC0B) | (1<<POAC0A) | (1<<PSEI0) | (1<<PEV0B) | (1<<PEV0A) | (1<<PRN01) | (1<<PRN00) | (1<<PEOP0);
	PIFR1 = (1<<POAC1B) | (1<<POAC1A) | (1<<PSEI1) | (1<<PEV1B) | (1<<PEV1A) | (1<<PRN11) | (1<<PRN10) | (1<<PEOP1);
	PIFR2 = (1<<POAC2B) | (1<<POAC2A) | (1<<PSEI2) | (1<<PEV2B) | (1<<PEV2A) | (1<<PRN21) | (1<<PRN20) | (1<<PEOP2);
}


