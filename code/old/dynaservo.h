#ifndef DYNASERVO_H
#define DYNASERVO_H

#include <inttypes.h>

// Anzahl der Segmente in der Schlange. TODO: dafür sorgen, dass diese Definition auch verwendet wird!!
//#define SNAKELENGTH 16
#include "../comdefs.h" // enthält alle definitionen, die wir für die Kommunikation brauchen

// Spezielle ID, falls man einen Befehl an alle Servos senden will
#define AX_12_BROADCAST_ID	0xfe

// Beim Empfangen mit Timeout:
#define AX_12_MAX_TRIALS	10
#define AX_12_RETRY_CAUSE	((ERROR_RECEIVE_TIMEOUT | ERROR_ID | ERROR_CHECKSUM_RECV))


//{{{ Dynamixel AX-12 Instruktionen laut Datenblatt

#define INSTR_PING			(0x01)
#define INSTR_READ_DATA		(0x02)
#define INSTR_WRITE_DATA	(0x03)
#define INSTR_REG_WRITE		(0x04)
#define INSTR_ACTION		(0x05)
#define INSTR_RESET			(0x06)
#define INSTR_SYNC_WRITE	(0x07) //0x83 laut Datenblatt
//}}}

//{{{ Dynamixel AX-12 Adress Table

// ITEM								Adress		Access			Initial Value 			Length (bytes)			Min		Max
#define	MODEL_NUMBER_L				(0X00)	//	RD			    12(0x0C)
#define	MODEL_NUMBER_H				(0X01)	//	RD			    0(0x00)
#define	MODEL_NUMBER	MODEL_NUMBER_L
#define	FIRMWARE_VERSION			(0X02)	//	RD			    ?
#define	ID							(0X03)	//	RD,W R			1(0x01)					1						0		253(0xfd)
#define	BAUD_RATE					(0X04)	//	RD,W R			1(0x01)					1						0		254(0xfe)
#define	RETURN_DELAY_TIME			(0X05)	//	RD,W R			250(0xFA)				1						0		254(0xfe)
#define	CW_ANGLE_LIMIT_L			(0X06)	//	RD,W R			0(0x00)					2						0		1023(0x3ff)
#define	CW_ANGLE_LIMIT_H			(0X07)	//	RD,W R			0(0x00)
#define	CW_ANGLE_LIMIT CW_ANGLE_LIMIT_L
#define	CCW_ANGLE_LIMIT_L			(0X08)	//	RD,W R			255(0xFF)				2						0		1023(0x3ff)
#define	CCW_ANGLE_LIMIT_H			(0X09)	//	RD,W R			3(0x03)
#define	CCW_ANGLE_LIMIT CCW_ANGLE_LIMIT_L
//#define							(0x0A)	//	-     			0(0x00)
#define	HIGHEST_LIMIT_TEMPERATURE	(0X0B)	//	RD,W R			85(0x55)				1						0		150(0x96)
#define	LOWEST_LIMIT_VOLTAGE		(0X0C)	//	RD,W R			60(0X3C)				1						50		(0x32) 250(0xfa)
#define	HIGHEST_LIMIT_VOLTAGE		(0X0D)	//	RD,W R			190(0xBE)				1						50		(0x32) 250(0xfa)
#define	MAX_TORQUE_L				(0X0E)	//	RD,W R			255(0XFF)				2						0		1023(0x3ff)
#define	MAX_TORQUE_H				(0X0F)	//	RD,W R			3(0x03)
#define	MAX_TORQUE MAX_TORQUE_L
#define	STATUS_RETURN_LEVEL			(0X10)	//	RD,W R			2(0x02)					1						0		2
#define	ALARM_LED					(0X11)	//	RD,W R			4(0x04)       			1						0		127(0x7f)
#define	ALARM_SHUTDOWN				(0X12)	//	RD,W R			4(0x04)       			1						0		127(0x7f)
//#define							(0X13)	//	RD,W R			0(0x00)       			1						0		1
#define	DOWN_CALIBRATION_L			(0X14)	//	RD    			?
#define	DOWN_CALIBRATION_H			(0X15)	//	RD    			?
#define	DOWN_CALIBRATION DOWN_CALIBRATION_L
#define	UP_CALIBRATION_L			(0X16)	//	RD    			?
#define	UP_CALIBRATION_H			(0X17)	//	RD    			?
#define	UP_CALIBRATION UP_CALIBRATION_L
#define	TORQUE_ENABLE				(0X18)	//	RD,W R			0(0x00)       			1						0		1
#define	LED							(0X19)	//	RD,W R			0(0x00)       			1						0		1
#define	CW_COMLIANCE_MARGIN			(0X1A)	//	RD,W R			0(0x00)       			1						0		254(0xfe)
#define	CCW_COMLIANCE_MARGIN		(0X1B)	//	RD,W R			0(0x00)       			1						0		254(0xfe)
#define	CW_COMLIANCE_SLOPE			(0X1C)	//	RD,W R			32(0x20)      			1						1		254(0xfe)
#define	CCW_COMLIANCE_SLOPE			(0X1D)	//	RD,W R			32(0x20)      			1						1		254(0xfe)
#define	GOAL_POSITION_L				(0X1E)	//	RD,W R			[Addr36]value 			2						0		1023(0x3ff)
#define	GOAL_POSITION_H				(0X1F)	//	RD,W R			[Addr37]value
#define	GOAL_POSITION GOAL_POSITION_L
#define	MOVING_SPEED_L				(0X20)	//	RD,W R			0             			2						0		1023(0x3ff)
#define	MOVING_SPEED_H				(0X21)	//	RD,W R			0
#define	MOVING_SPEED MOVING_SPEED_L
#define	TORQUE_LIMIT_L				(0X22)	//	RD,W R			[Addr14] value			2						0		1023(0x3ff)
#define	TORQUE_LIMIT_H				(0X23)	//	RD,W R			[Addr15] value
#define	TORQUE_LIMIT TORQUE_LIMIT_L
#define	PRESENT_POSITION_L			(0X24)	//	RD    			?
#define	PRESENT_POSITION_H			(0X25)	//	RD    			?
#define	PRESENT_POSITION PRESENT_POSITION_L
#define	PRESENT_SPEED_L				(0X26)	//	RD    			?
#define	PRESENT_SPEED_H				(0X27)	//	RD    			?
#define	PRESENT_SPEED PRESENT_SPEED_L
#define	PRESENT_LOAD_L				(0X28)	//	RD    			?
#define	PRESENT_LOAD_H				(0X29)	//	RD    			?
#define	PRESENT_LOAD PRESENT_LOAD_L
#define	PRESENT_VOLTAGE				(0X2A)	//	RD    			?
#define	PRESENT_TEMPERATURE			(0X2B)	//	RD    			?
#define	REGISTERED_INSTRUCTION		(0X2C)	//	RD,W R			0(0x00)       			1						0		1
//#define							(0X2D)	//	-     			0(0x00)
#define	MOVING						(0x2E)	//	RD    			0(0x00)
#define	LOCK						(0x2F)	//	RD,W R			0(0x00)       			1						1		1
#define	PUNCH_L						(0x30)	//	RD,W R			32(0x20)      			2						0		1023(0x3ff)
#define	PUNCH_H						(0x31)	//	RD,W R			0(0x00)
#define	PUNCH PUNCH_L

// TODO define back LEDs

//}}}

//{{{ Dynamixel AX-12 Baudrate Table

// Definiert, welche Baudrate setBaudrate() setzt
#define BAUD_1000000	1
#define BAUD_500000		3
#define BAUD_400000		4
#define BAUD_250000		7
#define BAUD_200000		9
#define BAUD_115200		16
#define BAUD_57600		34
#define BAUD_19200		103
#define BAUD_9600		207

//#define AX_12_BAUD_RATE AX_12_BAUD_RATE_19230_8
//
//#define AX_12_BAUD_RATE_1000000_0	0x01
//#define AX_12_BAUD_RATE_500000_0	0x03
//#define AX_12_BAUD_RATE_300000_0	0x04
//#define AX_12_BAUD_RATE_250000_0	0x07
//#define AX_12_BAUD_RATE_200000_0	0x09
//#define AX_12_BAUD_RATE_117647_1	0x10
//#define AX_12_BAUD_RATE_57142_9		0x22
//#define AX_12_BAUD_RATE_19230_8		0x67
//#define AX_12_BAUD_RATE_9615_4		0xcf
//}}}

void setBaudrate(uint8_t id, uint8_t baudCode);

uint8_t setAlarmLed(uint8_t errorBit);

uint8_t setServoGoal(uint8_t id, uint16_t pos);

uint8_t setServoID(uint8_t old_id, uint8_t new_id) ;

uint8_t servoTorqueEnable(uint8_t id, uint8_t en) ;
uint8_t servoTorqueEnableALL(uint8_t enable);

uint8_t setMovingSpeed(uint8_t id, uint16_t speed);

uint8_t setCWAngleLimit(uint8_t id, uint16_t angle);
uint8_t setCCWAngleLimit(uint8_t id, uint16_t angle);

uint8_t setLed_servo(uint8_t id, uint8_t status);

uint8_t get_temperature(uint8_t id, uint8_t *data);

uint8_t get_servo_position(uint8_t id, uint16_t *data);

uint8_t get_servo_goal(uint8_t id, uint16_t* data);
void setServoGoals(const uint16_t* pos);

void sendPacket(uint8_t id, uint8_t instruction, const uint8_t* parameters, uint8_t n_parameters);

uint8_t readData(uint8_t id, uint8_t adress, uint8_t *target, uint8_t length);

uint8_t ping(uint8_t id);

#endif /* DYNASERVO_H */
