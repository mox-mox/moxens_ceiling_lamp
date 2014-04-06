#ifndef PWM_H
#define PWM_H


#define R1 PSCOUT01
#define G1 PSCOUT11
#define B1 PSCOUT00

#define R2 PSCOUT10
#define G2 PSCOUT20
#define B2 PSCOUT21


void init_pwm();
inline void start_pwm();
inline void stop_pwm();








#endif /* PWM_H */
