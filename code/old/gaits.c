#include "gaits.h"
#include <math.h>
#include "main.h"


//float gait_parameters[] = { 0.0f, 0.0f, 100.0f, 100.0f, 0.0f, 0.0f,   0.0f,   0.0f, (M_PI/4.0f), 1.0f, 0.5f };		// Rectilinguar
//float gait_parameters[] = { 0.0f, 0.0f, 100.0f, 100.0f, 120.0f, 120.0f,   0.0f,   0.0f, (M_PI/4.0f), 1.0f, 0.5f };		// Rectilinguar kurve
//float gait_parameters[] = { 0.0f, 0.0f, 100.0f, 100.0f, 55.0f, 55.0f,   0.0f,   0.0f, (M_PI/4.0f), 1.0f, 0.5f };		// Rectilinguar kurve
//float gait_parameters[] = { 0.0f, 0.0f,  75.0f,  75.0f, 0.0f, 0.0f, 150.0f, 150.0f, (M_PI/4.0f), 0.5f, 0.5f };		// Sidewinding
//float gait_parameters[] = { 0.0f, 0.0f,  50.0f,  -50.0f, 150.0f, 0.0f, 0.0f, 150.0f, (M_PI/4.0f), 0.5f, 0.5f };		// Sidewinding Kurve



//{{{ Sidewinding

float piecewDiffGait(uint8_t n, float t)
{
	float phi = (params_piecewDiffGait[dphi_by_dn] * n) + (params_piecewDiffGait[dphi_by_dt] * t);
	if(n <= SNAKELENGTH/2)
	{
		if(n%2)
		{
			return params_piecewDiffGait[offset_odd_a] + (params_piecewDiffGait[amplitude_odd_a]*sin(phi));
		}
		else
		{
			return params_piecewDiffGait[offset_even_a] + (params_piecewDiffGait[amplitude_even_a]*sin(phi + params_piecewDiffGait[delta_a]));
		}
	}
	else
	{
		if(n%2)
		{
			return params_piecewDiffGait[offset_odd_b] + (params_piecewDiffGait[amplitude_odd_b]*sin(phi));
		}
		else
		{
			return params_piecewDiffGait[offset_even_b] + (params_piecewDiffGait[amplitude_even_b]*sin(phi + params_piecewDiffGait[delta_a]));
		}
	}
}

//}}}


// gaits ist eine Tabelle mit Funktionszeigern
// gaits[current_gait](segmentnummer, zeitpunkt) führt das aktuelle Gait aus
Gait gaits[] = { piecewDiffGait };

uint8_t gaitParameterInit()
{
	
	// by default snake is on halt
	gaitFlag = 0;
	
	// init parameter for piecewise differential gait
	params_piecewDiffGait[offset_even_a] = 0.0f;
	params_piecewDiffGait[offset_even_b] = 0.0f;  
	params_piecewDiffGait[amplitude_even_a] = 75.0f;
	params_piecewDiffGait[amplitude_even_b] = 75.0f; 
	params_piecewDiffGait[offset_odd_a] = 0.0f;
	params_piecewDiffGait[offset_odd_b] = 0.0f; 
	params_piecewDiffGait[amplitude_odd_a] = 150.0f; 
	params_piecewDiffGait[amplitude_odd_b] = 150.0f; 
	params_piecewDiffGait[delta_a] = (M_PI/4.0f);
	params_piecewDiffGait[delta_b] = (M_PI/4.0f);
	params_piecewDiffGait[dphi_by_dn] = 0.5f;
	params_piecewDiffGait[dphi_by_dt] = 0.5f;

	// time increment;
	gaitTimeIncStep = 0.2f;

	return SUCCESS;
}

uint8_t gaitInit(uint8_t gait)
{
	if(gait < sizeof(gaits))
	{
		gait_function = gaits[gait];
		return gaitParameterInit(gait);
	}
	else return FAILURE;
}






void calculateGait(float t)
{
	for(uint8_t n=1; n<=SNAKELENGTH; n++)
	{
		servoGoals[n-1]= 0x01FF+((int16_t) gait_function(n, t));// gait_function zeigt auf die Funktion, die das aktuelle Gait implementiert
	}
}


void sendGoals()
{
	for(uint8_t i=0; i<SNAKELENGTH; i++)
	{
		setServoGoal(i+1, servoGoals[i]);
	}
}
