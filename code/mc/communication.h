#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "main.h"
#include "comdefs.h"


void init_communication();
//void send_command(uint8_t command, uint8_t data);
void send_command(instruction command, const uint8_t data[3]);

#endif /* COMMUNICATION_H */
