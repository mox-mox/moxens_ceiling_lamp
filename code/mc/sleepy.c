#include "sleepy.h"
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//void init_sleepy()
//{
//	SBIT(
//	// Configure INT2 as negedge triggered
//	EICRA = (0<<ISC31) | (0<<ISC30) | (1<<ISC21) | (0<<ISC20) | (0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
//	EIMSK = (0<<INT3) | (1<<INT2) | (0<<INT1) | (0<<INT0);
//}
