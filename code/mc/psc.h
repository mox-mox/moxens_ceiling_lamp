#ifndef PSC_H
#define PSC_H


#define R1 PSCOUT01
#define G1 PSCOUT11
#define B1 PSCOUT00

#define R2 PSCOUT10
#define G2 PSCOUT20
#define B2 PSCOUT21


void init_psc();
inline void start_psc();
inline void stop_psc();








#endif /* PSC_H */
