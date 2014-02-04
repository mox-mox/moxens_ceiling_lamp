#include "dynaservo.h"


#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
//#include "math.h"
//#include <stdio.h>
#include <float.h>
#include "main.h"
#include <stddef.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif




//{{{
/* Kommunikationsprotokoll
 *
 * Datenpaket:
 *
 * - Byte: Paketbeginn
 * - Byte: Paketbeginn
 *   Byte: Ziel-ID
 * - Byte: Anzahl der Datenbytes im Paket: 1 (Befehl) + #Daten + 1 (Checksumme)
 * - Byte: Befehl
 * - Byte: { Daten }
 * - Byte: Checksumme: Bitweise negierte Addition mit Überlauf: Ziel-ID + Anzahl der Datenbytes + Befehl { + Daten }
 *
 *
 * Daten eines Befehls:
 * Byte: Befehl (es gibt also maximal 256 Befehle
 * Argument 1
 * ...
 * Argument n
 */
//}}}






//{{{
uint16_t receivePacket(uint8_t id_expected, uint8_t *data, uint8_t data_length)
{
	//Rueckgabewert ist der Fehler laut Datenblatt (untere 8bit) und eigene Fehler (obere 8 bit)
	uint16_t error = 0x0000;
	//~ uint8_t byte=0;

	//~ //_delay_ms(2);
	//~ sputc(0xdd);
	//~ sputc(0xdd);
	//~ while(byte != PACKAGE_BEGIN)				// erstes Startbit
	//~ {
		//~ if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT;  return error; }
		//~ sputc(byte);
	//~ }
	//~ byte=0;										// DAS _MUSS_ DA STEHEN
	//~ sputc(0xdd);
	//~ sputc(0xdd);
	//~ while(byte != PACKAGE_BEGIN)				// zweites Startbit
	//~ {
		//~ if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
		//~ sputc(byte);
	//~ }

	// The problem is that you still have a very noisy communication. Even though 
	// the communication seems to be clear we may receive more than 2 start bytes or even 
	// junk - thus a more tolerant algorithm is necessary to generally poll the bytes until 
	// something usefull comes in. Further more we count the bytes only in case
	// they seem to be equal to a start bytes. As soon as we receive (!) after at 
	// least 1 valid start byte, another byte we go on and pray that the packet wont break
	// in the mean time - which it normally does not. 

	uint8_t byte=PACKAGE_BEGIN;
	uint8_t packCnt = 0;
	while((byte == PACKAGE_BEGIN) || (packCnt < 1)) {
		if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
		if (byte == PACKAGE_BEGIN) {
			packCnt++;
		}
		//~ sputc(byte);
	}
	//~ sputc(0xDD);
	//~ sputc(packCnt);
	uint8_t id = byte;		// as we already received the first non start byte - 
							// consider it as ID and move on
	//~ if(ugetc_with_timeout(&id)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
	//~ sputc(id_expected);
	//~ sputc(id);

	uint8_t checksum = id;
	if(id != id_expected)
	{
		error |= ERROR_ID;
	}

	uint8_t length=0xbb;
	//~ sputc(length);
	if(ugetc_with_timeout(&length)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
	checksum += length;
	//~ sputc(length);
	length -=2;									// Gesammtlänge - 1 (error) - 1 (checksum)
	//sputc(length);

	if(length > data_length)					// Das übergebene Datenarray ist zu kurz
	{
		error |= ERROR_TOO_LONG_RESPONSE;
	}

	uint8_t foreign_error;
	if(ugetc_with_timeout(&foreign_error)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
	error |= ((uint16_t)foreign_error);
	checksum += foreign_error;

	//~ sputc(foreign_error);

	for(uint8_t i=0; i<length; i++)
	{
		if(i >= data_length)					// Falls wir im Array keinen Platz mehr haben
		{
			if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
			checksum += byte;				// müssen wir trotzdem noch die Checksumme berechnen
		}
		else
		{
			if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT; return error; }
			data[i] = byte;
			checksum += byte;
		}
	}
	if(ugetc_with_timeout(&byte)) { error |= ERROR_RECEIVE_TIMEOUT;  return error; }

	//~ sputc(checksum);

	// XXX: FIXED THIS
	//if(((uint8_t)(~((uint8_t)checksum))) != ((uint8_t)byte)) // <-- warning: comparison of promoted ~unsigned with unsigned [-Wsign-compare]
	// With:
	uint8_t not_checksum = ~checksum;
	if(not_checksum != byte) 			// warning: comparison of promoted ~unsigned with unsigned [-Wsign-compare]
	{
		error |= ERROR_CHECKSUM_RECV;
	}

	return error;
}
//}}}

//{{{
void sendPacket(uint8_t id, uint8_t instruction, const uint8_t* parameters, uint8_t n_parameters)
{
	uputc(PACKAGE_BEGIN);
	uputc(PACKAGE_BEGIN);
	uint8_t checksum=id;
	uputc(id);
	checksum += (n_parameters+2);
	uputc(n_parameters+2);
	checksum += instruction;
	uputc(instruction);
	for (uint8_t i=0; i < n_parameters; i++)
	{
		uint8_t p=parameters[i];
		uputc(p);
		checksum+=p;
	}
	checksum=~checksum;
	uputc(checksum);
}
//}}}

//{{{
void sendWritePacket(uint8_t id, uint8_t targetRegister, const uint8_t* parameters, uint8_t n_parameters)
{
	uputc(PACKAGE_BEGIN);
	uputc(PACKAGE_BEGIN);
	uint8_t checksum=id;
	uputc(id);
	checksum += (n_parameters+3);
	uputc(n_parameters+3);
	checksum += INSTR_WRITE_DATA;
	uputc(INSTR_WRITE_DATA);
	checksum += targetRegister;
	uputc(targetRegister);
	for (uint8_t i=0; i < n_parameters; i++)
	{
		uint8_t p=parameters[i];
		uputc(p);
		checksum+=p;
	}
	checksum=~checksum;
	uputc(checksum);
}
//}}}

//{{{
void broadcastWritePacket(const uint8_t* parameters, uint8_t n_parameters) // currently unused
{
	uputc(PACKAGE_BEGIN);
	uputc(PACKAGE_BEGIN);
	uint8_t checksum=AX_12_BROADCAST_ID;
	uputc(AX_12_BROADCAST_ID);
	checksum += (n_parameters+2);
	uputc(n_parameters+2);
	checksum += INSTR_SYNC_WRITE;
	uputc(INSTR_SYNC_WRITE);
	for (uint8_t i=0; i < n_parameters; i++)
	{
		uint8_t p=parameters[i];
		uputc(p);
		checksum+=p;
	}
	checksum=~checksum;
	uputc(checksum);
}
//}}}

//{{{
static inline uint8_t sendCommand(uint8_t id, uint8_t instruction, const uint8_t * parameter, uint8_t n_parameter)	// currently unused
{
	uint16_t error=0;
	for(uint8_t i=0; i<AX_12_MAX_TRIALS; i++)										// Senden, bis wir die Antwort sauber empfangen können
	{
		error=0;
		sendPacket(id, instruction, parameter, n_parameter);
		error = receivePacket(id, NULL, 0);											// Wir erwarten nur das Fehlerbyte

		if((error & AX_12_RETRY_CAUSE) == 0) break;	// Wenn es keinen Fehler gab, brauchen wir das Paket nicht nochmal senden
		_delay_ms(0.2f);
	}
	if(error)
	{
		send_error_packet(id, error);
	}
	return (uint8_t) error;
}
//}}}

//{{{
static inline uint8_t sendWriteCommand(uint8_t id, uint8_t targetRegister, const uint8_t * parameter, uint8_t n_parameter)
{
	uint16_t error=0;
	for(uint8_t i=0; i<AX_12_MAX_TRIALS; i++)										// Senden, bis wir die Antwort sauber empfangen können
	{
		sendWritePacket(id, targetRegister, parameter, n_parameter);
		error = receivePacket(id, NULL, 0);											// Wir erwarten nur das Fehlerbyte

		if((error & AX_12_RETRY_CAUSE) == 0) break;	// Wenn es keinen Fehler gab, brauchen wir das Paket nicht nochmal senden
		_delay_ms(0.2f);
	}
	if(error)
	{
		send_error_packet(id, error);
	}
	return (uint8_t) error;
}
//}}}

//{{{
uint8_t readData(uint8_t id, uint8_t adress, uint8_t *target, uint8_t length)
{
	// Kommando zum Datenlesen schicken
	uint8_t cmd[] = { adress, length };
	uint16_t error;
	for(uint8_t i=0; i<AX_12_MAX_TRIALS; i++)									// Senden, bis wir die Antwort sauber empfangen können
	{
		sendPacket(id, INSTR_READ_DATA, cmd, 2);

		// und die Antwort empfangen
		error = receivePacket(id, target, length);
		if(!(error & AX_12_RETRY_CAUSE)) break;	// Wenn es keinen Fehler gab, brauchen wir das Paket nicht nochmal senden
	}
	if(error)
	{
		send_error_packet(id, error);
	}
	return (uint8_t) error;
}
//}}}

//{{{
uint8_t get_servo_goal(uint8_t id, uint16_t *data)
{
	return readData(id, GOAL_POSITION, (uint8_t *) data, 2);
}
//}}}

//{{{
uint8_t setServoGoal(uint8_t id, uint16_t pos)
{
	return sendWriteCommand(id, GOAL_POSITION, (uint8_t *) &pos, 2);
}
//}}}

//{{{ TODO: Test this
//void setServoGoals(const uint16_t* pos)
//{
//	uputc(PACKAGE_BEGIN);
//	uputc(PACKAGE_BEGIN);
//	uint8_t checksum=AX_12_BROADCAST_ID;
//	uputc(AX_12_BROADCAST_ID);
//	//(L + 1) * N + 4 (L: Data length for each Dynamixel actuator, N: The number of Dynamixel actuators)
//	uint8_t n_parameters = 3*SNAKELENGTH +4;
//	checksum += (n_parameters);
//	uputc(n_parameters);				// Die +4 von der Zeile drüber brauchen wir nur hier
//
//	uputc(INSTR_SYNC_WRITE);
//	checksum += INSTR_SYNC_WRITE;
//
//	uputc(GOAL_POSITION);
//	checksum += GOAL_POSITION;
//	uputc(2);							// 2 Bytes pro Servo schreiben (uint16_t)
//	checksum+=2;
//	for (uint8_t i=0; i < SNAKELENGTH; i++)
//	{
//		uint8_t temp = i+1;
//		uputc(temp);
//		checksum+=(temp);
//
//		temp = (uint8_t)(pos[i]>>8);
//		uputc(temp);
//		checksum+=(temp);
//
//		temp = (uint8_t)pos[i];
//		uputc(temp);
//		checksum+=(temp);
//	}
//	checksum=~checksum;
//	uputc(checksum);
//}
//}}}

//{{{
uint8_t ping(uint8_t id)
{
	return sendCommand(id, INSTR_PING, NULL, 0);
}
//}}}

//{{{
uint8_t get_temperature(uint8_t id, uint8_t *data)
{
	return readData(id, PRESENT_TEMPERATURE,  data, 1);
}
//}}}

//{{{
uint8_t get_servo_position(uint8_t id, uint16_t *data)
{
	return readData(id, PRESENT_POSITION, (uint8_t *) data, 2);
}
//}}}

//{{{
uint8_t  setServoID(uint8_t old_id, uint8_t new_id)
{
	return sendWriteCommand(old_id, ID, &new_id, 1);
}
//}}}

//{{{
void setBaudrate(uint8_t id, uint8_t baudCode)
{
	sendWritePacket(id, BAUD_RATE, &baudCode, 1);	// Die Baudrate setzen
	_delay_ms(5);

	uint8_t byte=0;									// Die Antwort des Servos an den PC schicken...
	while(!ugetc_with_timeout(&byte))				// ...bis nichts mehr kommt
		sputc(byte);
}
//}}}

//{{{
uint8_t servoTorqueEnable(uint8_t id, uint8_t enable)
{
	return sendWriteCommand(id, TORQUE_ENABLE, &enable, 1);
}
//}}}

//{{{
uint8_t servoTorqueEnableALL(uint8_t enable)
{
    uint8_t error = 0;
	for(uint8_t i=1; i <= SNAKELENGTH; i++)
    {
        error |= servoTorqueEnable(i, enable);
    }
    return error;
}
//}}}

//{{{
uint8_t setMovingSpeed(uint8_t id, uint16_t speed)
{
	return sendWriteCommand(id, MOVING_SPEED, (uint8_t*) speed, 2);
}
//}}}

//{{{
uint8_t setCWAngleLimit(uint8_t id, uint16_t angle)
{
	return sendWriteCommand(id, CW_ANGLE_LIMIT, (uint8_t*) angle, 2);
}

uint8_t setCCWAngleLimit(uint8_t id, uint16_t angle)
{
	return sendWriteCommand(id, CCW_ANGLE_LIMIT, (uint8_t*) angle, 2);
}
//}}}

//{{{
uint8_t setLed_servo(uint8_t id, uint8_t status)
{	// Steuert die Led eines Servos direkt an
	return sendWriteCommand(id, LED, &status, 1);
}
//}}}

//{{{
uint8_t setAlarmLed(uint8_t errorBit)
{	// Setzt, bei welchen Fehlern die Leds der Servos leuchten
	uint8_t error = 0;
	for(uint8_t id = 1; id <= SNAKELENGTH; id++)
	{
		error |= sendWriteCommand(id, ALARM_LED, &errorBit, 1);
	}
	return error;
}
//}}}

