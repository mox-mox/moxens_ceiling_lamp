#ifndef SLEEPY_H
#define SLEEPY_H

// Slow down the µController as much as possible when the power is turned of, but leave timer1 running to measure time. Each 17 minutes the cpu wakes up and checks how much power is left in the big capacitor. When the power is low, save all values to the eeprom.

//*octave:4> fquartz = 4*1000000, fcpu=fquartz/256, ftimer=fcpu/256, ttimer=1/ftimer, Ttimer=ttimer*(2^16-1), Ttimer_min=Ttimer/60
//fquartz =  4000000
//fcpu =  15625
//ftimer =  61.035
//ttimer =  0.016384
//Ttimer =  1073.7
//Ttimer_min =  17.895


// Wenn Power zusammenbricht (INT2), clock maximal runterteilen, PSC ausschalten. S_PWR abschalten, falls gesetzt. Timer1 auf 

//void init_sleepy();















#endif /* SLEEPY_H */
