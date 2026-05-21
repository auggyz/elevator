/**
 * @file cabin.h
 */
#ifndef _CABIN_H
#define _CABIN_H

#include "timer.h"

#define PIN_MOTOR_CABIN_A  2
#define PIN_MOTOR_CABIN_B  3
#define PIN_MOTOR_DOORS_A  4
#define PIN_MOTOR_DOORS_B  5

#define CABIN_DOOR_OPEN  -1
#define CABIN_DOOR_CLOSE  1
#define CABIN_DOOR_STOP   0

void cabin_init         (int start);
int  cabin_current_floor();
int  cabin_move         (timer_ms& start, int to_floor, unsigned long floor_duration);
void cabin_stop         ();
void cabin_door         (int dir);

#endif