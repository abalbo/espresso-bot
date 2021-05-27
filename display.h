#ifndef display_h
#define display_h

#include "temperature.h"

#define DISP_PERIOD_MS 500      //display update period in ms

extern float setpoint;
extern int duty;

void display_init();
void display_bot_logo();
void display_wifi_connect();
void display_update();
void display_turn_off();

#endif