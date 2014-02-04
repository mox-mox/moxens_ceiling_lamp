#include "timer.h"
#include "main.h"

// Verwendung: auf antwort der dynamixelservos warten, timertick für main-loop
void timerInit()
{
	rx_wait_flag=0;
	srx_wait_flag=0;
	// timer0
	TCCR0A = 0<<COM0A1 | 0<<COM0A0 | 0<<COM0B1 | 0<<COM0B0 | 1<<WGM01 | 0<<WGM00;	// CTC mode, OCR0A disconnected, OCR0B disconnected
	TCCR0B = 0<<FOC0A  | 0<< FOC0B | 0<<WGM02  | 0<<CS02   | 1<<CS01  | 1<<CS00;	// Timer-Frequenz = F_CPU/64

	TCNT0 = 0;
	OCR0A = 122;																	// 16MHz/64/122 = 2049.2 Hz
	OCR0B = 0;
	TIMSK0 =  0<<OCIE0B | 1<<OCIE0A | 0<<TOIE0;
    TIFR0 = 0;
}

ISR (TIMER0_COMPA_vect)																// Wird mit 2049.2 Hz aufgerufen
{
	if ((stiffnessOff_flag> 1) && (!gaitFlag) && (PORTD & (1<<PD2)))
	{
		stiffnessOff_flag--;
	}
	if(rx_wait_flag)
	{
		rx_wait_flag--;
	}
	if(srx_wait_flag)
	{
		srx_wait_flag--;
	}

	if(gait_next_step_flag < 255)
	{
		gait_next_step_flag++;															// Den nächsten Zeitschritt des Gaits berechnen
	}

	if(status_packet_flag < 255)
	{
		status_packet_flag++;														// Wenn Flag bei 255 ist (& status_output_leven >0) Statuspaket senden
	}
}

