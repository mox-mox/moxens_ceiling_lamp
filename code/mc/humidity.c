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

	uint8_t freq_array[3];
	*freq_array=scaled_frequency;	// Write to freq_array[0] and freq_array[1] at once without breaking up the 16-bit variable.
	//freq_array[2]=0;				   Not neccesary. This byte will not be read anyways.
	send_command(humidity, freq_array);
}


void measure_humidity()
{
	enable_humidity_sensor();
	request_frequency_measurement();
}
