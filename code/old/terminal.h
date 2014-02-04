#ifndef TERMINAL_H
#define TERMINAL_H
#include <stdint.h>
#include "suart.h"
#include "gaits.h"
#include "dynaservo.h"
#include "main.h"
#include "../comdefs.h" // enthält alle definitionen, die wir für die Kommunikation mit der Schlange brauchen



#define ARGUMENT_MAX 32

uint8_t terminal();
uint8_t send_status_packet();
void send_error_packet(uint8_t id, uint16_t error);


#endif /* TERMINAL_H */
