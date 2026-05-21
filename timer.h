/**
 * @file timer.h
 */
#ifndef _TIMER_H
#define _TIMER_H

typedef unsigned long timer_ms;

void timer_reset  (timer_ms& timer);
bool timer_elapsed(timer_ms& timer, unsigned long limit);

#endif