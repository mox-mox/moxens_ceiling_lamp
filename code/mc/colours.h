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

inline void set_r1(colour_value r);
inline void set_g1(colour_value g);
inline void set_b1(colour_value b);

inline void set_r2(colour_value r);
inline void set_g2(colour_value g);
inline void set_b2(colour_value b);

inline uint8_t get_r1()
inline uint8_t get_g1()
inline uint8_t get_b1()

inline uint8_t get_r2()
inline uint8_t get_g2()
inline uint8_t get_b2()

#endif /* COLOURS_H */
