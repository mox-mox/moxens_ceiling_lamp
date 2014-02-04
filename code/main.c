/************************************************************************/
/*                                                                      */
/*                   Steuerung für einen Schlangenroboter               */
/*                                                                      */
/*              Gero Plettenberg, Manuel Rössler, Moritz Nöltner,       */
/*              WS2012/13, SS2013                                       */
/*                                                                      */
/*              http://www.TODO: Projektseite einfügen                  */
/*                                                                      */
/************************************************************************/

#include "main.h"


//void init();


int main(void)
{
	//init();															//Initialisierung ausfuehren

	volatile int i=0;

	for( ; ; )
	{
		i++;
	
//		//{{{ Kommunikation mit dem PC
//		// check whether there is data		
//		if (skbhit()) {
//			setLED_tail(1, 0);
//			//data = terminal();
//			setLED_tail(1, 1);
//		}
//
//
//		//{{{ Statuspaket
//
//		if(status_packet_flag == 255)
//
//		{
//
//            status_flag++;
//			if(status_output_level==1 && status_flag == 20) // Ungefähr 1 mal pro Sekunde Statuspaket senden
//			{
//				setLED_tail(2, 0);
//				send_status_packet();
//                status_flag = 0;
//				setLED_tail(2, 1);
//			}
//			status_packet_flag = 0;
//		}
//		//}}}
//
//		//{{{ Bewegungsgenerierung
//
//		//~ if ((gait_next_step_flag) && (gaitFlag))
//		if (gaitFlag)
//		{
//			//~ counter++;
//			gait_next_step_flag--;
//			calculateGait(timestep);
//			sendGoals();
//			timestep += gaitTimeIncStep;
//			//stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s
//		}
//		//~ // check if stiffness is overdue and should be switched off
//		if ((stiffnessOff_flag == 1) && (PORTD & (1<<PD2))) {
//			setSummer(1);
//			killStiffness();
//			stiffnessOff_flag = 0;
//			_delay_ms(100);
//			setSummer(0);
//		}
//		//~ //}}}
	}
}

//}}}


/****************************** Init Stuff *******************************/
//
////{{{
//void variablesInit()
//{
//	// Variablen setzen
//	// TODO: Variablen direkt aus dem EEPROM lesen
//	// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial#EEPROM
//	status_output_level = 0;
//	//status_output_level = 1;
//	status_packet_flag = 0;
//    status_flag = 0;
//	current_gait = 0;
//	gait_next_step_flag=0;
//	gaitInit(current_gait);
//	timestep = 0;
//	//stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s
//
//}
////}}}
//
////{{{
//void init()
//{
//	suartInit();					// serielle Ausgabe an PC
//	//uartInit(UART_BAUDRATE);
//	//uartInit(1000000ul);
//	uartInit();
//	variablesInit();
//	snakeInit();					// Schlange initialisieren
//	sei();
//
//
//	setLED_head(0, 1);				//Auge
//	setLED_head(1, 1);				//Auge
//
//	setLED_tail(0, 1);				//Status-LED Servo              grün
//	setLED_tail(1, 1);				//                              gelb
//	setLED_tail(2, 1);				//Error-LED error_signal()      rot-links
//	setLED_tail(3, 1);				//Status-LED Controller         rot-rechts
//	setSummer(0);
//
//	setLED_tail(3, 0);				//Status-LED Controller ON
//
//	setLED_head(0, 0);				//Auge
//	setLED_head(1, 0);				//Auge
//
//	//~ setLED_tail(0, 0);				//Status-LED Servo              grün
//	//~ setLED_tail(1, 0);				//                              gelb
//	//~ setLED_tail(2, 0);				//Error-LED error_signal()      rot-links
//	//~ setLED_tail(3, 0);				//Status-LED Controller         rot-rechts
//
//	//goodMorning();
//}
////}}}
//
////{{{
//void setupServoID(uint8_t id)
//{
//	suartInit();
//	uartInit(UART_BAUDRATE);
//	sei();
//	sputc(0xdd);
//	sputc(0xdd);
//	sputc(0xdd);
//	sputc(0xdd);
//	_delay_ms(2000);
//	setServoID(4, id);			// setServoID(ALTE_ID, NEUE_ID)
//	for( ; ; )
//		;	// Am Ende warten
//}
////}}}
//
////{{{
//void setupServoBaudrate()
//{
//	sputc(0xbb);
//	sputc(0xbb);
//	sputc(0xbb);
//	sputc(0xbb);
//	_delay_ms(2000);
//	setBaudrate(16, BAUD_19200);	// Die Baudrate setzen
//	_delay_ms(2000);
//	sputc(0xbb);
//	sputc(0xbb);
//	sputc(0xbb);
//	sputc(0xbb);
//	for( ; ; )
//		;	// Am Ende warten
//}
////}}}
////
//
