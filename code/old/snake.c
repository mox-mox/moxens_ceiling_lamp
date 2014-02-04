#include "snake.h"

//{{{
void snakeInit() // initialize snake
{
    DDRD |= (1<<2);		// An D2 hängt die Stromversorgung der Servos


    //LEDs und Summer aktivieren
    DDRB |= (1<<2);     //LED tail
    DDRB |= (1<<3);     //LED tail
    DDRB |= (1<<4);     //LED tail
    DDRD |= (1<<7);     //LED tail
    DDRD |= (1<<5);     //Summer
    DDRC |= (1<<4);     //LED eye
    DDRC |= (1<<5);     //LED eye

    //servoTorqueEnableALL(1); // FIXME
}
//}}}

//{{{
uint8_t stopServos()
{
	gaitFlag = 0;
	deactivateServos();
	return SUCCESS;
}

uint8_t startServos()
{
	activateServos();
	return SUCCESS;
}

uint8_t execGoodMorning()
{
	gaitFlag = 0;
	goodMorning();
	return SUCCESS;
}

uint8_t killStiffness()
{
	gaitFlag = 0;
	return servoTorqueEnableALL(0);
}

uint8_t setStiffness()
{
	stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s

	return servoTorqueEnableALL(1);
}

//}}}

//{{{ What for?
//void deactivateALL()
//{
//    deactivateServos();
//
//    setLED_head(0, 1);
//    setLED_head(1, 1);
//
//    setLED_tail(0, 1);
//    setLED_tail(1, 1);
//    setLED_tail(2, 1);
//    setLED_tail(3, 1);
//
//    setSummer(0);
//}
//}}}

//{{{
void activateServos() // set servo activation pin to HIGH
{
	PORTD |= (1<<PD2);	// Die Servos aktivieren
    setLED_tail(0, 0);
	_delay_ms(1000); // Warten, bis Servos gebootet haben
	setSummer(1);
	_delay_ms(100);
	setSummer(0);
	stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s

}
//}}}

//{{{
void deactivateServos() // set servo activation pin to LOW
{
	PORTD &= ~(1<<PD2);
    setLED_tail(0, 1);
	setSummer(1);
	_delay_ms(500);
	setSummer(0);
	stiffnessOff_flag=0;
}
//}}}

//{{{
void setLED_tail(uint8_t nr, uint8_t status) //LEDs B2, B3, B4, D7
{
    switch(nr) {
        case 0:
            PORTB &= ~(1<<2);
            if(status) PORTB |= (1<<2);
            break;
        case 1:
            PORTB &= ~(1<<3);
            if(status) PORTB |= (1<<3);
            break;
        case 2:
            PORTB &= ~(1<<4);
            if(status) PORTB |= (1<<4);
            break;
        case 3:
            PORTD &= ~(1<<7);
            if(status) PORTD |= (1<<7);
            break;
    }
}


void setLED_head(uint8_t nr, uint8_t status) //LEDs C4, C5
{
    switch(nr) {
        case 0:
            PORTC &= ~(1<<4);
            if(status) PORTC |= (1<<4);
            break;
        case 1:
            PORTC &= ~(1<<5);
            if(status) PORTC |= (1<<5);
            break;
    }
}

//}}}

//{{{
void setSummer(uint8_t status)
{ //Summer D5
    if(!status) PORTD |= (1<<5);
    else PORTD &= ~(1<<5);
}
//}}}

//{{{
void goodMorning()
{	//reset-Funktion, um die Schlange in ausgestreckten Modus zu bekommen
	stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s

	for (uint8_t id=1; id <= SNAKELENGTH; id++)
	{
		setServoGoal(id, 0x01ffu);
	}
	_delay_ms(1000);
}
//}}}
