// Long live the glue logic!
#ifndef HUMIDITY_H
#define HUMIDITY_H

#define S_PWR PD5

void init_humidity();
void measure_humidity();

//extern uint16_t last_humidity_measurement; // In case of a broken transmission, the result can be sent again


#endif /* HUMIDITY_H */
