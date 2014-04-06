#ifndef COLOURS_H
#define COLOURS_H

void init_colours();

typedef uint8_t colour_value;

typedef struct colour_struct;
{
	colour_value r;
	colour_value g;
	colour_value b;
} color;

void update_colour1();
void update_colour2();

//void set_rgb1(colour rgb);
//void set_rgb2(colour rgb);

inline void set_r1(colour_value r);
inline void set_g1(colour_value g);
inline void set_b1(colour_value b);

inline void set_r2(colour_value r);
inline void set_g2(colour_value g);
inline void set_b2(colour_value b);

#endif /* COLOURS_H */
