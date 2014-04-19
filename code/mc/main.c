/************************************************************************/
/*                                                                      */
/*                      UART test program                               */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#include "main.h"
#include <avr/io.h>
//#include <avr/interrupt.h>
#include "colours.h"
#include "humidity.h"
#include "communication.h"





void init();


int main(void)
{
	init();															//Initialisierung ausfuehren

	volatile int i=0;

	for( ; ; )
	{
		i++;
	}
}



/****************************** Init Stuff *******************************/

void init()
{
	// set all Pins as High-Z inputs
	DDRB=0x00;
	DDRC=0x00;
	DDRD=0x00;
	DDRE=0x00;

	PORTB=0x00;
	PORTC=0x00;
	PORTD=0x00;
	PORTE=0x00;

	init_colours();

	init_humidity();

	init_communication();
}
