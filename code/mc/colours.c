#include "colours.h"
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

colour new_colour1={255, 255, 255};	// Set to full brightness
colour new_colour2={255, 255, 255};	// as a default.

void init_colours()
{
	init_psc();
	update_colour1_values();
	update_colour2_values();
}


//{{{
uint16_t convert_r_to_psc_value(colour_value r) // TODO
{
	r=r;
	return 0;
}
uint16_t convert_g_to_psc_value(colour_value g) // TODO
{
	g=g;
	return 0;
}
uint16_t convert_b_to_psc_value(colour_value b) // TODO
{
	b=b;
	return 0;
}
//}}}

//{{{
colour_value convert_psc_value_to_r(uint16_t foo) // TODO
{
	foo=foo;
	return 0;
}
colour_value convert_psc_value_to_g(uint16_t foo) // TODO
{
	foo=foo;
	return 0;
}
colour_value convert_psc_value_to_b(uint16_t foo) // TODO
{
	foo=foo;
	return 0;
}
//}}}

//{{{
void update_colour1_values()
{
	set_value_r1(new_colour1.r);
	set_value_g1(new_colour1.g);
	set_value_b1(new_colour1.b);
}

void update_colour2_values()
{
	set_value_r2(new_colour2.r);
	set_value_g2(new_colour2.g);
	set_value_b2(new_colour2.b);
}
//}}}
