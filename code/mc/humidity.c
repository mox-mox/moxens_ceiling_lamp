#include "humidity.h"
#include "frequency_meter.h"
#include <avr/io.h>

void measurement_complete_handler(uint16_t scaled_frequency);


void init_humidity()
{
	// Set the pin that gates the power to the sensor as output.
	SBIT(  DDRD, S_PWR) = 1;
	// And disable the sensor until it is needed.
	disable_humidity_sensor();

	init_freq_counter(&measurement_complete_handler);
}

void enable_humidity_sensor()
{
	// Power is active low, so set the port to 0 to activate.
	SBIT(  PORTD, S_PWR) = 0;
}

void disable_humidity_sensor()
{
	// Power is active low, so set the port to 1 to deactivate.
	SBIT(  PORTD, S_PWR) = 1;
}

void measurement_complete_handler(uint16_t scaled_frequency)
{
	if(!freq_meter_is_measuring())
		disable_humidity_sensor();
	//TODO: Do something with scaled_frequency.
}


void measure_humidity()
{
	enable_humidity_sensor();
	request_freq_measurement();
}
