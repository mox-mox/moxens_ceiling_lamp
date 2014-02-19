#ifndef FREQ_METER_H
#define FREQ_METER_H
// Measure a frequency at pin T1 using timer0 and timer1. Inspired by Atmel Appnote AVR205

// Set the time intervall during which the measurement is done
#define FREQ_METER_TIMER_WINDOW 0.1d

#if !defined F_CPU
	#error "Freq_meter needs to know the CPU frequency, so please define F_CPU"
#endif




void init_freq_counter();

void start_freq_measurement();

//sets the function to call when the measurement is done
typedef void (*callback_function)(uint16_t freq_divided by_10);
void set_measurement_complete_action(callback_function callback);



#endif /* FREQ_METER_H */

















//divider =  256
//t_max_extended =  0.11469
//octave:23> t_max_extended = (1/F_CPU/divider^C
//octave:23> F_CPU=4*1000000, required_gate_timer_cycles=7, divider = 256, t=1/(F_^CU/divider); t_max = t*256; t_max_extended = t_max*required_gate_timer_cycles
//octave:23> t_max_extended = 1/(F_CPU/divider)*required_gate_timer_cycles
//t_max_extended =    4.4800e-04
//*octave:24> t_max_extended = 1/(F_CPU/divider)*256*required_gate_timer_cycles
//t_max_extended =  0.11469
//octave:25> required_gate_timer_cycles = floor(0.1/ (1/(F_CPU/divider)*256)
//> )
//required_gate_timer_cycles =  6
//*octave:26> required_gate_timer_cycles = floor(0.1/ (1/(F_CPU/divider)*256))
//required_gate_timer_cycles =  6
//*octave:27> F_CPU=4*1000000, divider = 256, t=1/(F_CPU/divider); t_max = t*256; t_max_extended = t_max*required_gate_timer_cycles
//F_CPU =  4000000
//divider =  256
//t_max_extended =  0.098304
//*octave:28> required_gate_timer_cycles = floor(0.1/ (1/(F_CPU/divider)*256))+1
//required_gate_timer_cycles =  7
//octave:29> F_CPU=4*1000000, divider = 256, t=1/(F_CPU/divider); t_max = t*256; t_max_extended = t_max*required_gate_timer_cycles
//F_CPU =  4000000
//divider =  256
//t_max_extended =  0.11469
//*octave:30> required_gate_timer_cycles = floor(1/ (10/(F_CPU/divider)*256))+1
//required_gate_timer_cycles =  7
//*octave:31> required_gate_timer_cycles = floor((F_CPU/divider)*256)/10)+1
//parse error:
//
//  syntax error
//
//>>> required_gate_timer_cycles = floor((F_CPU/divider)*256)/10)+1
//                                            ^
//
//*octave:31> required_gate_timer_cycles = floor((F_CPU/divider)*256/10)+1
//required_gate_timer_cycles =  400001
//octave:32> required_gate_timer_cycles = floor(1/ (10/(F_CPU/divider)*256))+1
//required_gate_timer_cycles =  7
//octave:33> required_gate_timer_cycles = floor(F_CPU/(2560*divider))
//required_gate_timer_cycles =  6
//*octave:34> required_gate_timer_cycles = floor(F_CPU/(2560*divider))+1
//required_gate_timer_cycles =  7
//octave:35>




//	*octave:70> F_CPU=4*1000000, divider=256, timer_window=0.1, counter_steps_max=256;
//	required_gate_timer_cycles = floor(F_CPU/(counter_steps_max*divider*(1/timer_window)))+1,
//	counter_steps= floor((timer_window*F_CPU/divider - 256*required_gate_timer_cycles + 256) +0.5),
//	t_extended = 1/(F_CPU/divider)*256*(required_gate_timer_cycles-1) + 1/(F_CPU/divider)*counter_steps
//
//	F_CPU =  4000000
//	divider =  256
//	timer_window =  0.10000
//	required_gate_timer_cycles =  7
//	counter_steps =  27
//	t_extended =  0.10003
