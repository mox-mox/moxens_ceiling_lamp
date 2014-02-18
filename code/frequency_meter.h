#ifndef FREQ_METER_H
#define FREQ_METER_H

#define FREQ_METER_TIMER_WINDOW 0.1d

#if !defined F_CPU
	#error "Freq_meter needs to know the CPU frequency, so please define F_CPU"
#endif




void init_freq_counter();

void start_freq_measurement();

//sets the function to call when the measurement is done
typedef void (*callback_function)(uint16_t freq_divided by_10);
void set_complete_action(callback_function callback);



#endif /* FREQ_METER_H */

















//divider =  256
//t_max_extended =  0.11469
//octave:23> t_max_extended = (1/F_CPU/divider^C
//octave:23> F_CPU=4*1000000, extender=7, divider = 256, t=1/(F_^CU/divider); t_max = t*256; t_max_extended = t_max*extender
//octave:23> t_max_extended = 1/(F_CPU/divider)*extender
//t_max_extended =    4.4800e-04
//*octave:24> t_max_extended = 1/(F_CPU/divider)*256*extender
//t_max_extended =  0.11469
//octave:25> extender = floor(0.1/ (1/(F_CPU/divider)*256)
//> )
//extender =  6
//*octave:26> extender = floor(0.1/ (1/(F_CPU/divider)*256))
//extender =  6
//*octave:27> F_CPU=4*1000000, divider = 256, t=1/(F_CPU/divider); t_max = t*256; t_max_extended = t_max*extender
//F_CPU =  4000000
//divider =  256
//t_max_extended =  0.098304
//*octave:28> extender = floor(0.1/ (1/(F_CPU/divider)*256))+1
//extender =  7
//octave:29> F_CPU=4*1000000, divider = 256, t=1/(F_CPU/divider); t_max = t*256; t_max_extended = t_max*extender
//F_CPU =  4000000
//divider =  256
//t_max_extended =  0.11469
//*octave:30> extender = floor(1/ (10/(F_CPU/divider)*256))+1
//extender =  7
//*octave:31> extender = floor((F_CPU/divider)*256)/10)+1
//parse error:
//
//  syntax error
//
//>>> extender = floor((F_CPU/divider)*256)/10)+1
//                                            ^
//
//*octave:31> extender = floor((F_CPU/divider)*256/10)+1
//extender =  400001
//octave:32> extender = floor(1/ (10/(F_CPU/divider)*256))+1
//extender =  7
//octave:33> extender = floor(F_CPU/(2560*divider))
//extender =  6
//*octave:34> extender = floor(F_CPU/(2560*divider))+1
//extender =  7
//octave:35>




//	*octave:70> F_CPU=4*1000000, divider=256, timer_window=0.1, counter_steps_max=256;
//	extender = floor(F_CPU/(counter_steps_max*divider*(1/timer_window)))+1,
//	counter_steps= floor((timer_window*F_CPU/divider - 256*extender + 256) +0.5),
//	t_extended = 1/(F_CPU/divider)*256*(extender-1) + 1/(F_CPU/divider)*counter_steps
//
//	F_CPU =  4000000
//	divider =  256
//	timer_window =  0.10000
//	extender =  7
//	counter_steps =  27
//	t_extended =  0.10003
