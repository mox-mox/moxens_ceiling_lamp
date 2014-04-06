#ifndef FREQUENCY_METER_H
#define FREQUENCY_METER_H
#include <stdint.h>
/******************************************************************************/
/*                                                                            */
/*    Measure a frequency at pin T1 using timer0 and timer1                   */
/*    Inspired by Atmel Appnote AVR205                                        */
/*                                                                            */
/*    Principle:                                                              */
/*     - Timer0 measures the "gate open" time, for example 0.1 seconds        */
/*       * 0.1 second is more than one timer cycle, so the timer has          */
/*         to run more than one cycle, see required_gate_timer_cycles         */
/*     - Counter1 counts the positive flanks during the gate open time        */
/*     => The actual frequency is (positive flanks)/(gate open time)          */
/*                                                                            */
/*    Usage:                                                                  */
/*     - Define FREQ_METER_GATE_OPEN_TIME in seconds                          */
/*     - Set measurement_complete_action. This function will be called        */
/*       when the measurement is done. The argument is the number of          */
/*       cycles counted, not the actual frequency (because that might         */
/*       might not fit into 16 bit. Use get_actual_frequency() to get         */
/*       the actual frequency)                                                */
/*     - Call request_frequency_measurement()                                 */
/*     - Done. Any further processing of the result should be done by         */
/*       the callback function.                                               */
/*                                                                            */
/******************************************************************************/


// Measure a frequency at pin T1 using timer0 and timer1. Inspired by Atmel Appnote AVR205



// Set the time intervall during which the measurement is done
#define FREQ_METER_GATE_OPEN_TIME 0.1f

#if !defined F_CPU
	#error "Frequency_meter needs to know the CPU frequency, so please define F_CPU somewhere appropriate."
#endif

#define FREQ_METER_INVALID_FREQUENCY 0xFFFF
typedef void (*frequency_measurement_callback_function)(uint16_t frequency_multiplied_by_gate_open_time);

// Set up everything. This function only needs to be called once (or, when the timer/counter 0/1,
// PC2 or PC3 were used elsewhere). It also sets the function to call when the measurement is done.
void init_frequency_meter(frequency_measurement_callback_function frequency_measurement_complete_callback);


// Tell the module it should measure a frequency. The callback function will be called when the
// measurement is done, so this is basically a fire and forget solution
void request_frequency_measurement();

// Convenience function to undo the scaling.
// Usually you should just use the scaled frequency because the scaling is constant and allows to
// fit into a 16 bit variable.
uint32_t get_actual_frequency(uint16_t scaled_frequency);

uint8_t frequency_meter_is_measuring();


#endif /* FREQUENCY_METER_H */
