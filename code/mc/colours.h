#ifndef COLOURS_H
#define COLOURS_H

#include "psc.h"
#include <avr/io.h>


void init_colours();

typedef uint8_t colour_value;

typedef struct colour_struct
{
	colour_value r;
	colour_value g;
	colour_value b;
} colour;

extern colour new_colour1;
extern colour new_colour2;

void update_colour1_values();
void update_colour2_values();


uint16_t convert_r_to_psc_value(colour_value r); // TODO
uint16_t convert_g_to_psc_value(colour_value g); // TODO
uint16_t convert_b_to_psc_value(colour_value b); // TODO

colour_value convert_psc_value_to_r(uint16_t foo); // TODO
colour_value convert_psc_value_to_g(uint16_t foo); // TODO
colour_value convert_psc_value_to_b(uint16_t foo); // TODO


//{{{
inline void set_value_r1(colour_value r)
{
	R1=convert_r_to_psc_value(r);
}
inline void set_value_g1(colour_value g)
{
	G1=convert_g_to_psc_value(g);
}
inline void set_value_b1(colour_value b)
{
	B1=convert_b_to_psc_value(b);
}

inline void set_value_r2(colour_value r)
{
	R2=convert_r_to_psc_value(r);
}
inline void set_value_g2(colour_value g)
{
	G2=convert_g_to_psc_value(g);
}
inline void set_value_b2(colour_value b)
{
	B2=convert_b_to_psc_value(b);
}
//}}}

//{{{
inline uint8_t get_r1_value()
{
	return convert_psc_value_to_r(R1);
}
inline uint8_t get_g1_value()
{
	return convert_psc_value_to_g(G1);
}
inline uint8_t get_b1_value()
{
	return convert_psc_value_to_b(B1);
}
inline uint8_t get_r2_value()
{
	return convert_psc_value_to_r(R2);
}
inline uint8_t get_g2_value()
{
	return convert_psc_value_to_g(G2);
}
inline uint8_t get_b2_value()
{
	return convert_psc_value_to_b(B2);
}
//}}}


#endif /* COLOURS_H */
