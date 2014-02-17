#ifndef HUMIDITY_H
#define HUMIDITY_H

#define S_PWR PD5
#define FREQ_IN PC2 // Using T0
//#define FREQ_IN PC3 // Using T1

void get_humidity();
void init_humidity();



#endif /* HUMIDITY_H */
