#include "colours.h"
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_colours()
{
	init_psc();

	uint8_t full_brightness_array[3]={255, 255, 255};
	set_rgb1_values(full_brightness_array);
	set_rgb2_values(full_brightness_array);

	start_psc();
}
