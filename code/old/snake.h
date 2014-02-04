#ifndef SNAKE_H
#define SNAKE_H

#include "main.h"


void snakeInit();

void deactivateALL();

void activateServos();

void deactivateServos();

void setLED_tail(uint8_t nr, uint8_t status);

void setSummer(uint8_t status);

void goodMorning();
uint8_t execGoodMorning();

void setLED_head(uint8_t nr, uint8_t status);


uint8_t killStiffness();
uint8_t setStiffness();

uint8_t startServos();
uint8_t stopServos();

#endif /* SNAKE_H */
