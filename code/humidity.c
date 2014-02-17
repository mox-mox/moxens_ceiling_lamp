#include "humidity.h"
#include <avr/io.h>

//PORTA |= (1 << MEINBIT);    /* setzt Bit 2 an PortA auf 1 */
//PORTA &= ~(1 << MEINBIT);   /* loescht Bit 2 an PortA */




void init_humidity()
{
	// Use T0/T1 to get frequency input from the sensor, no pullup
	DDRC &=~(1 << FREQ_IN);
	PORTC &=~(1 << FREQ_IN);

	// Disable the power for the sensor. It is active low.
	DDRD |= (1 << S_PWR);
	disable_humidity_sensor();
}

void enable_humidity_sensor()
{
	// Power is active low, so set the port to 0 to activate
	PORTD &=~(1 << S_PWR);
}

void disable_humidity_sensor()
{
	// Power is active low, so set the port to 1 to deactivate
	PORTD |= (1 << S_PWR);
}


// Frequenzmessung:
// - Signal liegt an T0 an. Signal taktet Timer/Counter0
// - Messung starten: 
//		* Timer/Counter0 wird auf 0 gesetzt und gehalten
//		* Timer/Counter1 wird auf 0 gesetzt.
//		* Timer/Counter1 wird gestartet
//		* Timer/Counter0 wird freigegeben
//		* Wenn Timer/Counter0 bei 255=TOP=MAX angekommen ist, wird der Stand von Timer/Counter1 gemessen.
//		-> Wert von Timer/Counter1 ist Reziprok zur Frequenz
void measure_humidity();
