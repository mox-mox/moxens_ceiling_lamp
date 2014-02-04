#ifndef GAITS_H
#define GAITS_H

#include <stdint.h>
#include "../comdefs.h"


#define MAX_PWDG 12

#define	offset_even_a 0
#define offset_even_b 1
#define amplitude_even_a 2
#define amplitude_even_b 3
#define offset_odd_a 4
#define offset_odd_b 5
#define	amplitude_odd_a 6
#define amplitude_odd_b 7
#define delta_a 8
#define delta_b 9
#define dphi_by_dn 10
#define dphi_by_dt 11

float params_piecewDiffGait[MAX_PWDG];

float gaitTimeIncStep;
uint8_t gaitFlag;

// Gait ist ein Zeiger auf eine Gait-Funktion
typedef float (*Gait) (uint8_t n, float t);

Gait gait_function;

float gait_parameters[NUMBER_OF_GAIT_PARAMETERS];

uint8_t gaitInit(uint8_t gait);
//uint8_t gaitParameterInit(uint8_t gait);



void calculateGait(float t);									// Berecnet den nächten Schritt des Gaits und legt ihn servoGoals ab

uint16_t servoGoals[SNAKELENGTH];

void sendGoals();







#endif /* GAITS_H */
