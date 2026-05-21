/**
 * @file timer.cpp
 */

#include <wino.h>
#include "timer.h"

void timer_reset(timer_ms& timer) {
  timer = millis();
}

bool timer_elapsed(timer_ms& timer, unsigned long limit) {
  if (millis() - timer < limit) return false;
  timer_reset(timer);
  return true;
}