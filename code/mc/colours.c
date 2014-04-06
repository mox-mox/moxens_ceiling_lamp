#include "colours.h"
#include "pwm.h"

colour new_colour1={255, 255, 255};
colour new_colour2={255, 255, 255};

void init_colours()
{
	init_pwm();
	set_colour1(new_colour1);
	set_colour2(new_colour2);
}


//{{{
uint16_t convert_r_to_psc_value(colour_value r);
uint16_t convert_g_to_psc_value(colour_value g);
uint16_t convert_b_to_psc_value(colour_value b);
//}}}

//{{{
colour_value convert_psc_value_to_r(uint16_t foo);
colour_value convert_psc_value_to_g(uint16_t foo);
colour_value convert_psc_value_to_b(uint16_t foo);
//}}}

//{{{
void update_colour1()
{
	set_r1(new_colour1.r);
	set_g1(new_colour1.g);
	set_b1(new_colour1.b);
}

void update_colour2()
{
	set_r1(new_colour2.r);
	set_g1(new_colour2.g);
	set_b1(new_colour2.b);
}
//}}}


//{{{
inline void set_r1(colour_value r)
{
	R1=convert_r_to_psc_value(r);
}
inline void set_g1(colour_value g);
{
	G1=convert_g_to_psc_value(g);
}
inline void set_b1(colour_value b);
{
	B1=convert_b_to_psc_value(b);
}

inline void set_r2(colour_value r);
{
	R2=convert_r_to_psc_value(r);
}
inline void set_g2(colour_value g);
{
	G2=convert_g_to_psc_value(g);
}
inline void set_b2(colour_value b);
{
	B2=convert_b_to_psc_value(b);
}
//}}}

//{{{
inline uint8_t get_r1()
{
	return convert_psc_value_to_r(R1);
}
inline uint8_t get_g1()
{
	return convert_psc_value_to_g(G1);
}
inline uint8_t get_b1()
{
	return convert_psc_value_to_b(B1);
}
inline uint8_t get_r2()
{
	return convert_psc_value_to_r(R2);
}
inline uint8_t get_g2()
{
	return convert_psc_value_to_g(G2);
}
inline uint8_t get_b2()
{
	return convert_psc_value_to_b(B2);
}
//}}}
