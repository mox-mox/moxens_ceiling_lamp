#include "terminal.h"
#include "mc_pc_comm.h"
#include "gaits.h"
#include <stddef.h>	// Da sollte der NULL-Pointer definiert sein :)
//#include "dynaservo.h"
#include "snake.h"
#include <stdlib.h>


static void send_to_pc(uint8_t command, uint8_t* ticketno, uint8_t *data, uint8_t length);
static void send_ack(uint8_t *ticketno);
static void send_nak(uint8_t *ticketno, uint8_t error);

static float fpt2fl(const int16_t fpt);
static int16_t fl2fpt(const float fl);

static int16_t assInt16(const uint8_t highByte, const uint8_t lowByte);
static void diassInt16(const int16_t complVal, uint8_t* highByte, uint8_t* lowByte);

static uint8_t argBuffer[ARGUMENT_MAX];

//{{{
uint8_t terminal()
{	// terminal sollte ein Paket aus dem Puffer lesen und darauf reagieren
	uint8_t retval = SUCCESS;

	// The problem is that you still have a very noisy communication. Even though 
	// the communication seems to be clear we may receive more than 2 start bytes or even 
	// junk - thus a more tolerant algorithm is necessary to generally poll the bytes until 
	// something usefull comes in. Further more we count the bytes only in case
	// they seem to be equal to a start bytes. As soon as we receive (!) after at 
	// least 1 valid start byte, another byte we go on and pray that the packet wont break
	// in the mean time - which it normally does not. 

	uint8_t byte=PACKAGE_BEGIN;
	uint8_t ticketno[4];
	uint8_t packCnt = 0;
	uint8_t checksum = 0;
	while((byte == PACKAGE_BEGIN) || (packCnt < 1)) {
		if(sgetc_with_timeout(&byte)) { retval |= TERMINAL_ERROR_READ_TIMEOUT; return retval; }
		if (byte == PACKAGE_BEGIN) {
			packCnt++;
		}
		//~ sputc(byte);
	}
	// now read 4 ticket bytes and store them for later processing
	for (uint8_t i=0; i<4; i++) {
		ticketno[i] = byte;
		checksum += byte;
		// post read
		if(sgetc_with_timeout(&byte)) { retval |= TERMINAL_ERROR_READ_TIMEOUT; return retval; }
	}
	// read the command length byte
	if (byte < 2) { retval |= TERMINAL_BAD_PACKAGE_LENGTH; send_nak(ticketno, retval); return retval; }
	
	uint8_t argMax = byte-2;	// Anzahl der Argumente des Befehls = Paketlänge - 1 (Befehl) -1 (Checksumme)
	checksum += byte;
	//{{{
	// read the command byte itself
	if(sgetc_with_timeout(&byte)) { retval |= TERMINAL_ERROR_READ_TIMEOUT; return retval; }
		
	uint8_t command = byte;
	checksum += byte;
	// fetch arguments from buffer
	for (unsigned int arg = 0; arg < argMax; arg++) {
		if(sgetc_with_timeout(&byte)) { retval |= TERMINAL_ERROR_READ_TIMEOUT; return retval; }
		argBuffer[arg] = byte;
		checksum += byte;
	}
	// the checksum comes always last
	if(sgetc_with_timeout(&byte)) { retval |= TERMINAL_ERROR_READ_TIMEOUT; return retval; }
	// filter out invalid checksums - send NAK in case it did not work
	if (byte != checksum) { retval |= TERMINAL_ERROR_INVALID_PACKAGE; send_nak(ticketno, retval); return retval; }
	
	switch(command)
	{
		case NO_COMMAND:	// this is a simple ping to check the logic
			// check for correct number of arguments
			if (argMax != NO_COMMAND_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			// just enjoy and virtually nothing
			send_ack(ticketno);
			break;		
		case STOP_SERVOS:	// Schlange abschalten
			// check for correct number of arguments
			if (argMax != STOP_SERVOS_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			status_output_level = 0;
			retval |= stopServos();
			send_ack(ticketno);
			break;
		case KILL_STIFFNESS:	// Schlange sanft stoppen;
			// check for correct number of arguments
			if (argMax != KILL_STIFFNESS_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			retval |= killStiffness();
			stiffnessOff_flag= 0;
			send_ack(ticketno);
			break;
		case START_SERVOS: // Switch on servo controler
			if (argMax != START_SERVOS_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			retval |= startServos();
			send_ack(ticketno);
			break;			
		case SET_STIFFNESS:	// Schlange sanft stoppen;
			// check for correct number of arguments
			if (argMax != SET_STIFFNESS_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			retval |= setStiffness();
			send_ack(ticketno);
			break;
		case MOVE_SNAKE: // start gait computation
			if (argMax != MOVE_SNAKE_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			stiffnessOff_flag= STIFFNESS_OFF; // set stiffness off flag to 15s
			setStiffness();
			gaitFlag = 1;
			send_ack(ticketno);
			break;			
		case HALT_SNAKE: // stop gait computation
			if (argMax != HALT_SNAKE_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			gaitFlag = 0;
			send_ack(ticketno);
			break;			
		case EXEC_MORNING: // executes good morning function
			if (argMax != EXEC_MORNING_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			retval |= execGoodMorning();
			send_ack(ticketno);
			break;			
		case STATUS_LEVEL:	// Statusausgabe kontrollieren
			// check for correct number of arguments
			if (argMax != STATUS_LEVEL_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			if(argBuffer[0]==0)
			{
				status_output_level = 0;
			}
			else if(argBuffer[0] == 1)
			{
				send_status_packet();
			}
			else if(argBuffer[0] == 2)
			{
				//send_status_packet();
				status_output_level = 1;
			}
			else
			{
				retval |= TERMINAL_ERROR_OUT_OF_RANGE;	// FIXME: Bessere Meldung
				send_nak(ticketno, retval);
				return retval;
			}
			send_ack(ticketno);
			break;
		case SET_GAIT_INC_STEP: // set parameters for differentiable gait function
			// check for correct number of arguments
			if (argMax != SET_GAIT_INC_STEP_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
		
			gaitTimeIncStep = fpt2fl(assInt16(argBuffer[0], argBuffer[1]));
			send_ack(ticketno);

			break;
		case SET_DIFF_GAIT_PARAM: // set parameters for differentiable gait function
			// check for correct number of arguments
			if (argMax != SET_DIFF_GAIT_PARAM_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }

			params_piecewDiffGait[offset_even_a] = fpt2fl(assInt16(argBuffer[0], argBuffer[1]))*10.0f;
			params_piecewDiffGait[offset_even_b] = fpt2fl(assInt16(argBuffer[2], argBuffer[3]))*10.0f;
			params_piecewDiffGait[amplitude_even_a] = fpt2fl(assInt16(argBuffer[4], argBuffer[5]))*10.0f;
			params_piecewDiffGait[amplitude_even_b] = fpt2fl(assInt16(argBuffer[6], argBuffer[7]))*10.0f;
			params_piecewDiffGait[offset_odd_a] = fpt2fl(assInt16(argBuffer[8], argBuffer[9]))*10.0f;
			params_piecewDiffGait[offset_odd_b] = fpt2fl(assInt16(argBuffer[10], argBuffer[11]))*10.0f;
			params_piecewDiffGait[amplitude_odd_a] = fpt2fl(assInt16(argBuffer[12], argBuffer[13]))*10.0f;
			params_piecewDiffGait[amplitude_odd_b] = fpt2fl(assInt16(argBuffer[14], argBuffer[15]))*10.0f;
			params_piecewDiffGait[delta_a] = fpt2fl(assInt16(argBuffer[16], argBuffer[17]));
			params_piecewDiffGait[delta_b] = fpt2fl(assInt16(argBuffer[18], argBuffer[19]));
			params_piecewDiffGait[dphi_by_dn] = fpt2fl(assInt16(argBuffer[20], argBuffer[21]));
			params_piecewDiffGait[dphi_by_dt] = fpt2fl(assInt16(argBuffer[22], argBuffer[23]));
			send_ack(ticketno);
			break;
		case GET_GAIT_INC_STEP: // executes good morning function
			if (argMax != GET_GAIT_INC_STEP_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			send_ack(ticketno);
			diassInt16(fl2fpt(gaitTimeIncStep), &argBuffer[0], &argBuffer[1]);
			send_to_pc(GET_GAIT_INC_STEP,ticketno, argBuffer, 2);
			break;			
		case GET_DIFF_GAIT_PARAM: // executes good morning function
			if (argMax != GET_DIFF_GAIT_PARAM_ARGS) { retval |= TERMINAL_ERROR_WRONG_NUMBER_OF_ARGUMENTS; send_nak(ticketno, retval); return retval; }
			send_ack(ticketno);
			diassInt16(fl2fpt(params_piecewDiffGait[offset_even_a]/10.0f),&argBuffer[0], &argBuffer[1]);
			diassInt16(fl2fpt(params_piecewDiffGait[offset_even_b]/10.0f),&argBuffer[2], &argBuffer[3]);
			diassInt16(fl2fpt(params_piecewDiffGait[amplitude_even_a]/10.0f),&argBuffer[4], &argBuffer[5]);
			diassInt16(fl2fpt(params_piecewDiffGait[amplitude_even_b]/10.0f),&argBuffer[6], &argBuffer[7]);
			diassInt16(fl2fpt(params_piecewDiffGait[offset_odd_a]/10.0f),&argBuffer[8], &argBuffer[9]);
			diassInt16(fl2fpt(params_piecewDiffGait[offset_odd_b]/10.0f),&argBuffer[10], &argBuffer[11]);
			diassInt16(fl2fpt(params_piecewDiffGait[amplitude_odd_a]/10.0f),&argBuffer[12], &argBuffer[13]);
			diassInt16(fl2fpt(params_piecewDiffGait[amplitude_odd_b]/10.0f),&argBuffer[14], &argBuffer[15]);
			diassInt16(fl2fpt(params_piecewDiffGait[delta_a]),&argBuffer[16], &argBuffer[17]);
			diassInt16(fl2fpt(params_piecewDiffGait[delta_b]),&argBuffer[18], &argBuffer[19]);
			diassInt16(fl2fpt(params_piecewDiffGait[dphi_by_dn]),&argBuffer[20], &argBuffer[21]);
			diassInt16(fl2fpt(params_piecewDiffGait[dphi_by_dt]),&argBuffer[22], &argBuffer[23]);
			send_to_pc(GET_DIFF_GAIT_PARAM,ticketno, argBuffer, 24);
			break;			

		default:
			// command unknown - report and exit
			retval |= TERMINAL_ERROR_UNKNOWN_COMMAND; 
			send_nak(ticketno, retval); 
			return retval;
			break;
//}}}
	}
	// congratulations !! we survived the trip the package was accepted and processed
	// this needs to be reported firmly
	// exit on succes
	return retval;
}
//}}}

//{{{
static float fpt2fl(const int16_t fpt) {
	return ((float) fpt) / (512.0f);
}
//}}}

//{{{
static int16_t fl2fpt(const float fl) {
	return ((int16_t) (fl * 512.0f));
}
//}}}


//{{{
static int16_t assInt16(const uint8_t highByte, const uint8_t lowByte) {
	int16_t complVal = (highByte << 8) | (lowByte);
	return complVal;
}

//}}}

//{{{
static void diassInt16(const int16_t complVal, uint8_t* highByte, uint8_t* lowByte) {
	*highByte = (uint8_t) (complVal >> 8);
	*lowByte = (uint8_t) complVal;
}

//}}}

//{{{
static void send_to_pc(uint8_t command, uint8_t *ticketno, uint8_t *data, uint8_t length)
{
	uint8_t checksum = 0;
	sputc(PACKAGE_BEGIN);
	sputc(PACKAGE_BEGIN);
	for(uint8_t i=0; i<4; i++)
	{
		if (ticketno != NULL) {
			sputc(ticketno[i]);
			checksum+=ticketno[i];
		} else {
			sputc(0);
		}
	}

	sputc(length+2);
	checksum+=(length+2);

	sputc(command);
	checksum+=command;

	for(uint8_t i=0; i<length; i++)
	{
		sputc(data[i]);
		checksum += data[i];
	}
	sputc(checksum);
}
//}}}






// Getestet:

//{{{
void send_error_packet(uint8_t id, uint16_t error)
{
	uint8_t error_array[] = { id, 0x00, 0x00 };
	error_array[1] = (uint8_t) (error>>8);
	error_array[2] = (uint8_t) error;
	send_to_pc(ERROR_PACKET, NULL, error_array, 3);
}
//}}}

//{{{
inline static void send_ack(uint8_t* ticketno)
{
	send_to_pc(ACK, ticketno, NULL, 0);
}
//}}}

//{{{
inline static void send_nak(uint8_t* ticketno, uint8_t error)
{
	//~ send_to_pc(NAK, ticketno, NULL, 0);
	//~ error = error;
	send_to_pc(NAK, ticketno, &error, 1);
}
//}}}

//{{{
uint8_t send_status_packet()
{
	uint8_t error=0;
	uint16_t buffer;

	sputc(PACKAGE_BEGIN);
	sputc(PACKAGE_BEGIN);
	// Statuspakete bekommen immer Ticketnummer 0
	sputc(0x00);
	sputc(0x00);
	sputc(0x00);
	sputc(0x00);

	// Die Länge ist das Erste, was in die Checksumme eingeht, daher können wir sie hier einfach als Länge missbrauchen
	uint8_t checksum = 5*SNAKELENGTH + 2;					// #Nutzdaten + 1(Befehl) +1(Checksumme)
	sputc(checksum);

	sputc(STATUS_PACKET);

	checksum += STATUS_PACKET;	// Befehl noch zur checksumme addieren

	for(uint8_t i=0; i<SNAKELENGTH; i++)
	{
		error |= get_servo_position((i+1), &buffer);
		sputc((uint8_t) (buffer>>8));
		checksum += (uint8_t) (buffer>>8);
		sputc((uint8_t) buffer);
		checksum += (uint8_t) buffer;
	}

	for(uint8_t i=0; i<SNAKELENGTH; i++)
	{
		buffer = servoGoals[i];
		sputc((uint8_t) (buffer>>8));
		checksum += (uint8_t) (buffer>>8);
		sputc((uint8_t) buffer);
		checksum += (uint8_t) buffer;
	}

	for(uint8_t i=0; i < SNAKELENGTH; i++)
	{
		uint8_t temperature;
		error |= get_temperature((i+1), &temperature);
		sputc(temperature);
		checksum += temperature;
	}

	sputc(checksum);
	return error;
}
//}}}
