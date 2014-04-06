#include "frequency_meter.h"
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// This function will be called when the measurement is completed
// It could for example set a flag or send the results to another device
// Argument: The measured frequency divided by 10
// (because the full value might not fit into an unit16_t :) )
frequency_measurement_callback_function measurement_complete_action=NULL;
volatile uint8_t measurement_requested;



void init_frequency_meter(frequency_measurement_callback_function frequency_measurement_complete_callback);
{
	// set T1/PC3 as input
	SBIT(  DDRC, DDRC3) = 0;
	SBIT( PORTC,PORTC3) = 0;
	// set T0/PC2 as input. This is only necessary because they are connected by a wire on my board.
	SBIT(  DDRC, DDRC2) = 0;
	SBIT( PORTC,PORTC2) = 0;


	// initialise everything that can be initialised but don't start the timer and counter yet

	// counter1 counts the edges of the input signal
	// disconnect the OC1A/OC1B and count up normally
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
	// no noise cancelling, no clock yet
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
	// no force output compare
	TCCR1C = (0<<FOC1A) | (0<<FOC1B);

	// timer0 measures 0.1 s. It will overflow during this period so it has to run multiple times
	// running in normal mode, the pins are disconnected
	TCCR0A = (0<<COM0A1)| (0<<COM0A0)| (0<<COM0B1)| (0<<COM0B0)| (0<<WGM01)| (0<<WGM00);
	// no force output compare, no clock yet
	TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0 = counter_preload;
	// no interrupts pending yet
	TIFR0 = 0xff;
	// only allow overflow interrupt
	TIMSK0 = (0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);


	measurement_complete_action=callback;
}



void request_frequency_measurement()
{
	uint8_t sreg = SREG;
	cli();
	if(	SBIT( TIMSK0, TOIE0 ) )		// The overflow interrupt enable bit is only set while measuring
	{
		measurement_requested = 1;	// Start measurement once no matter how many requestes are made.
		SREG = sreg;
	}
	else							// If no measurement is going on, we can start one directly
	{
		SREG = sreg;
		start_measurement();
	}
}



// Some compile time constants...
const int clock_divider = 1024;
const int counter_max_steps = 255;
// How many times the timer has to overflow during the measurement.
const int required_gate_timer_cycles =
	F_CPU/(counter_max_steps*clock_divider*(1/FREQ_METER_GATE_OPEN_TIME)) + 1;
// Running the timer (required_gate_timer_cycles*256 steps) would give a window that is slightly to
// long, so the timer gets a preload for the first run.
const int counter_steps = FREQ_TIMER_WINDOW*F_CPU/(double) divider -
	256d*(double) required_gate_timer_cycles + counter_max_steps + 0.5d;
const int counter_preload = counter_max_steps - counter_steps;



// Do the setup that has to be done everytime and start measuring at positive-going edge of the signal;
void start_measurement()
{
	// Timer0 measures 0.1 s. It will overflow during this period so it has to run multiple times
	// It will be started in the timer1 overflow interrupt when there is a positive edge on the signal
	// Preload the timer for the first run, so it will measure exactly 0.1s
	TCNT0 = counter_preload;
	// No interrupts pending yet
	TIFR0 = 0xff;
	// Only allow overflow interrupt
	TIMSK0 = (0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);


	// Use counter1 to determine when to actually start measuring
	// Next count shall cause an overflow (to emulate the pin change interrupt)
	TCNT1 = 0xffff;
	// No interrupts pending yet
	TIFR1 = 0xff;
	// Only allow overflow interrupt
	TIMSK1 = (0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);
	// Start the counter: no noise cancelling, clock on rising edge at T1 pin
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (1<<CS11) | (1<<CS10);

	return;
}



ISR(TIMER1_OVF_vect) // Use Counter1 as edge detector to know when to start the measurement
{
	// Start the timer: no force output compare, clock from F_CPU/1024
	TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	// If counter1 overflows, we know the frequency was to high so the result is not valid

	// Disable this interrupt because it is not needed any more
	TIMSK1 = (0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);
}



volatile uint8_t remaining_gate_timer_cycles;
ISR(TIMER0_OVF_vect)
{
	// The timer cycle takes only a fraction of the time the measurement takes so to get a longer
	// intervall it has to run multiple times.  See: required_gate_timer_cycles
	// Remaining_gate_timer_cycles is set at the start of measurement.
	// When it reaches 0 the measurement is finished.
	if(!--remaining_gate_timer_cycles) // Fold decrement and check. When finished, stop the timers
	{                                  //and start the callback function.
		// Stop the frequency counter.
		TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
		// And stop the gate_timer.
		TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00);

		// Save the results to a variable so other uses of the timer won't corrupt the data.
		uint16_t frequency = TCNT1;
		uint8_t is_overflowed = SBIT( TIFR1, TOV1);
		// Disable overflow interrupt. This also means that a new conversion can be started (TIMSK0 &= ~(1<<TOIE0);)
		SBIT( TIMSK0, TOIE0 ) = 0;
		sei(); // Any futher processing is nonblocking.
		// Start the callback.
		// No check for NULL is done to save performance. To reach this code the init function has
		// to have run so m._complete_action should be set.
		// If an overflow occured FREQ_METER_INVALID_FREQUENCY is handed over and signals the error.
		measurement_complete_action( is_overflowed ? FREQ_METER_INVALID_FRRQUENCY : &frequency );

		// If another measurement is requested, start it now.
		if(measurement_requested)
		{
			measurement_requested=0;
			start_measurement();
		}
	}
}










uint32_t get_actual_frequency(uint16_t scaled_frequency)
{
	return (scaled_frequency/FREQ_METER_GATE_OPEN_TIME);
}


uint8_t frequency_meter_is_measuring()
{
	return (SBIT( TIMSK0, TOIE0 ) | measurement_requested);

}
