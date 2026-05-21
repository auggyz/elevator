/**
 * @file program.cpp
 * @brief Main elevator control program — 6-floor Arduino simulation.
 *
 * State machine:
 *   OPENED → CLOSING → MOVING → OPENING → OPENED (loop)
 *   Any state → STOPPED via '#' key → OPENED via '#' again
 */

#include "floor.h"
#include "cabin.h"

#define TIME_OPENED      6000
#define TIME_DOORS       1700
#define TIME_FLOOR_SHORT 7150
#define TIME_FLOOR_LONG  7700

#define FLOOR_NUM 6

floor_info building[FLOOR_NUM] = {
  //                               --NeoPixel--  --ResistorLadder--
  // title          key  disp  def  ledUp ledDn  callUp  callDn  pressed
  { "Parking  " ,  '*',  -1, false,   9,    0,     84,     -1,    0 },
  { "RDC      " ,  '0',   0, true ,  11,   10,    101,     93,    0 },
  { "1er etage" ,  '1',   1, false,  13,   12,    118,    110,    0 },
  { "2eme etage",  '2',   2, false,   1,    2,    135,    127,    0 },
  { "3eme etage",  '3',   3, false,   3,    4,    152,    144,    0 },
  { "4eme etage",  '4',   4, false,   5,    6,    169,    161,    0 }
};

enum states {
  STATE_OPENED,
  STATE_CLOSING,
  STATE_MOVING,
  STATE_OPENING,
  STATE_STOPPED
};

states   state   = STATE_OPENED;
timer_ms timer;
int      target  = -1;
bool     stopped = false;

unsigned long movetime();

void setup() {
  Serial.begin(9600);
  cabin_init(
    floor_init(building, FLOOR_NUM)
  );
}

void loop() {
  const char* status = nullptr;

  floor_readbtns();

  char key = floor_getkey();
  if (key == '#') {
    stopped = !stopped;
    if (stopped) {
      cabin_stop();
      cabin_door(CABIN_DOOR_STOP);
      state = STATE_STOPPED;
    } else {
      state = STATE_OPENED;
    }
  }

  switch (state) {

    case STATE_STOPPED:
      status = "(ARRET URGENCE)";
      break;

    case STATE_OPENED:
      target = floor_requested(cabin_current_floor());
      status = "(waiting...)   ";
      if (timer_elapsed(timer, TIME_OPENED) && target >= 0) {
        cabin_door(CABIN_DOOR_CLOSE);
        state = STATE_CLOSING;
      }
      break;

    case STATE_CLOSING:
      cabin_door(CABIN_DOOR_CLOSE);
      status = "(closing doors)";
      if (timer_elapsed(timer, TIME_DOORS)) {
        cabin_door(CABIN_DOOR_STOP);
        state = STATE_MOVING;
      }
      break;

    case STATE_MOVING:
      status = "(moving)       ";
      if (cabin_move(timer, target, movetime()) == target) {
        cabin_stop();
        state = STATE_OPENING;
      }
      break;

    case STATE_OPENING:
      status = "(opening doors)";
      cabin_door(CABIN_DOOR_OPEN);
      if (timer_elapsed(timer, TIME_DOORS)) {
        cabin_door(CABIN_DOOR_STOP);
        state = STATE_OPENED;
      }
      break;
  }

  floor_feedback(cabin_current_floor(), status);
}

unsigned long movetime() {
  auto cur      = cabin_current_floor();
  int  distance = abs(target - cur);
  if (distance == 0) return 0;

  bool odd  = (cur % 2) != 0;
  bool down = target < cur;

  unsigned long base = (down && odd) || (!down && !odd)
    ? TIME_FLOOR_SHORT
    : TIME_FLOOR_LONG;

  return base * distance;
}