/************************************************************************/
/*                                                                      */
/*                      UART test program                               */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                                                                      */
/************************************************************************/
#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "communication.h"
#include "colours.h"
#include "humidity.h"





void init();


int main(void)
{
	init();															//Initialisierung ausfuehren

	//SBIT(DDRD, DDD2)=1;	// remove
	//volatile uint8_t foo;
	for( ; ; )
	{
		//PORTD=255;
		//PORTD=0;
	}
}



/****************************** Init Stuff *******************************/

void init()
{
	cli();

	init_colours();

	init_humidity();

	init_communication();
	sei();
}
