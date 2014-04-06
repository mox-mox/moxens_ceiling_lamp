#include "humidity.h"
#include "main.h"
#include "frequency_meter.h"
#include "communication.h"
#include <avr/io.h>

void measurement_complete_handler(uint16_t scaled_frequency);

void enable_humidity_sensor();
void disable_humidity_sensor();

void init_humidity()
{
	// Set the pin that gates the power to the sensor as output.
	SBIT(  DDRD, S_PWR) = 1;
	// And disable the sensor until it is needed.
	disable_humidity_sensor();

	init_frequency_meter(&measurement_complete_handler);
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

//volatile uint16_t last_humidity_measurement; // In case of a broken transmission, the result can be sent again
void measurement_complete_handler(uint16_t scaled_frequency)
{
	if(!frequency_meter_is_measuring())
		disable_humidity_sensor();

	//last_humidity_measurement=scaled_frequency;

	send_command(humidity1, (uint8_t) scaled_frequency>>8);
	send_command(humidity2, (uint8_t) scaled_frequency);
}


void measure_humidity()
{
	enable_humidity_sensor();
	request_frequency_measurement();
}
