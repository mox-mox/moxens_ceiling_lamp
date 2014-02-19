#include "frequency_meter.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// This function will be called when the measurement is completed
// It could for example set a flag or send the results to another device
// Argument: The measured frequency divided by 10 (because the full value would not fit into an unit16_t :) )
callback_function measurement_complete_action=NULL;

void set_measurement_complete_action(callback_function callback)
{
	measurement_complete_action=callback;
}

volatile uint8_t remaining_gate_timer_cycles;
ISR(TIMER0_OVF_vect)
{
	// The timer cycle takes only a fraction of the time the measurement takes so to get a longer intervall it has to run multiple times.
	// See: required_gate_timer_cycles
	// Remaining_gate_timer_cycles is set at the start of measurement. When it reaches 0 the measurement is finished.
	if(!--remaining_gate_timer_cycles) // Fold decrement and check. When finished, stop the timers and start the callback function.
	{
		// stop the frequency counter
		TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
		// and stop the gate_timer
		TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);

		// check for overflow of the frequency counter
		if(TIFR1 & (1<<TOV1)) // if an overflow occured
		{
			sei();
			measurement_complete_action(0x0000);	// signal error by sending a zero
		}
		else // no overflow occured
		{
			uint16_t freq = TCNT1;					// from now on, it is ok if TCNT1 is clobbered because the value is saved
			sei();									// No need to block interrupts while proccessing the result
			measurement_complete_action(freq);		// return the measured frequency
		}
	}
}






void init_freq_counter()// counter1
{
	// disconnect the OC1A/OC1B and count up normally
	TCCR1A = (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
	// no noise cancelling, clock on rising edge at T1 pin
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (1<<CS11) | (1<<CS10);
	// no force output compare
	TCCR1C = (0<<FOC1A) | (0<<FOC1B);

	// no interrupts should start now
	TIFR1 = 0xff;
	// only allow timer overflow interrupt
	TIMSK1 = (0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);
}

// if counter1 overflows, we know the frequency was to high for the time slot so the result is not valid




// some compile time constants...
const int clock_divider = 1024;
const int counter_max_steps = 256;
// how many times the timer overflows during the measurement
const int required_gate_timer_cycles = F_CPU/(counter_max_steps*clock_divider*(1/FREQ_METER_TIMER_WINDOW)) + 1;
// running the timer extend*256 steps would give a window that is slightly to long, so the timer gets a preload for the first run
const int counter_steps = FREQ_TIMER_WINDOW*F_CPU/(double) divider - 256d*(double) required_gate_timer_cycles + counter_max_steps + 0.5d;
const int runtime_delay_correction = 2
const int counter_preload = counter_max_steps - (counter_steps + runtime_delay_correction);

void init_gate_timer()							// timer0
{
	// running in normal mode, the pins are disconnected
	TCCR0A = (0<<COM0A1)| (0<<COM0A0)| (0<<COM0B1)| (0<<COM0B0)| (0<<WGM01)| (0<<WGM00);
	// no force output compare, clock from F_CPU/1024
	TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
	TCNT0 = counter_preload;
	// no interrupts pending yet
	TIFR0 = 0xff;
	// only allow overflow interrupt
	TIMSK0 = (0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);





}











unsigned int x48_cnt_to_four_ints;

unsigned int freq_cntr_freq_divd_by_10;

/*! \brief  AVR peripheral's initialization function
 *
 *  This function sends a command byte to the connected
 *  peripheral and waits for a returned status code.
 *
 *  \note  The peripheral must be initialized in advance.
 *
 */
void freq_cntr_init(void)
{
	// Initialize  16 bit FREQ_CNTR counts incoming positive edges on T0 input pin
	FREQ_CNTR_CTRLA = 0;							// Normal 16 bit mode OVF at 0xffff
	//Interrupts must be enabled for this code to work properly
}



//! GATE_CNTR is used as the time base for the 0.1 second sampling gate

/*! \brief  When interrupts are used, this function is called to start the measurement process.\n
 *	\A Pin Change of state on T0 will cause the 16-bit frequency counter to start.\n
 *
 *  - This function is not used during Busy Wait (polling) \n
 *
 */
void freq_cntr_start_measurement(void)
{
//! To detect a rising edge on PB6 with minimum latentcy, a Pin change interrupt is now enabled
	PIN_CHG_MASK = (1<<PIN_CHG_INT);							// for megax8;
	GEN_INT_MASK = (1<<PIN_CHG_INT_ENAB);
}



ISR(PIN_CHG_vect)
{
	if ((PIN & (1<<PIN_NO)) != 0)
	{
		PIN_CHG_MASK    = (0<<PIN_CHG_INT);
		GEN_INT_MASK &= ~(1<<PIN_CHG_INT_ENAB);
		GATE_CNTR_INT_FLAG_REG   |= (1<<GATE_CNTR_OVF_FLAG);							// clear GATE_CNTR overflow flag if set

// Enable FREQ_CNTR to count incoming signal on Tiny x61:PB6, Megax8: PD5

		FREQ_CNTR_COUNT_HIGH  = 0x00;							// clear counter upper 8 bits
		FREQ_CNTR_COUNT_LOW  = 0x00;							// clear counter lower 8
		FREQ_CNTR_CTRLB = (1<<CLOCK_SEL_2)|(1<<CLOCK_SEL_1);							// select FREQ_CNTR input clock

//The following counter pre-load was determined by the 0.1 second/512 usec period = 195.3 counts
//				TCNT1 = 255-195;// GATE_CNTR up counter loaded with this value to generate 0.1 sec delay
		GATE_CNTR_COUNT = 0;

		GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);


		GATE_CNTR_CTRLB = (1<<CLOCK_SEL_02)|(1<<CLOCK_SEL_00);								// divide by 1024, the largest divisor possible in MEGAx8
		x48_cnt_to_four_ints = 0;							//Clear the x48 counter that counts 4 interrupts before closing gate
	}
	// else detected a negative-going pin change int, so ignore this
}


////////////////////////////////

ISR(GATE_CNTR_OVF_vect)
{
	x48_cnt_to_four_ints++;
	if(x48_cnt_to_four_ints == 3) GATE_CNTR_COUNT = 0xff-13;
	if(x48_cnt_to_four_ints >= 4)
	{
		FREQ_CNTR_CTRLB = 0;							// stop FREQ_CNTR
		GATE_CNTR_CTRLB = 0;

//	ovf_test_and_return_result(); //places result into freq_cntr_freq_divd_by_10
// Test TIFR for 16-bit overflow.  If overrange return  0xFFFF

		if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) != 0)
		{
			// Test GATE_CNTR_OVF_FLAG for 16-bit overflow =  0xFFFF

			freq_cntr_freq_divd_by_10 = 0xFFFF;								// This is to return a OVF condition
		}
		else
		{
			freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);
		}
	}
}
