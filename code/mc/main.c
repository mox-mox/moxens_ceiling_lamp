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
#include "communicate.h"

//{{{

//void delay_1ms( u16 i )
//{
//  do{
//    _delay_ms( 1 );
//  }while( --i );
//}
//
//
//int main( void )
//{
//  init_uart0( (u16)(F_CPU / BAUD / 16 - 0.5) );
//  sei();
//  delay_1ms( 300 );
//  uputs0("Hallo Peter\n\r");
//  uputs0("UART buffer test\n\r");
//
//  for(;;){
//    if( !ukbhit0()){
//      continue;
//    }
//    delay_1ms( 10000 );        			// wait 10s
//						// to see rx buffer overflow
//    do{
//      uputchar0( ugetchar0());			// send without waiting
//      if( !utx0_ready() ){
//	if( !ukbhit0()){
//	  break;
//	}
//	uputchar0( '.' );			// to see tx buffer size
//	do{
//	  uputchar0( ugetchar0());		// send with busy waiting
//	}while( ukbhit0());
//      }
//    }while( ukbhit0());
//    uputchar0( '\n' );
//    uputchar0( '\r' );
//  }
//}

//}}}




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
