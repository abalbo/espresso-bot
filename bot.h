#ifndef bot_h
#define bot_h

#include "temperature.h"

#define BOT_UPDATE_PERIOD 5000  //period between scan messages in ms
// Initialize Telegram BOT
#define BOT_TOKEN "1741818478:AAFJ0xVM6x__3iqyeIG_TFUyRaYPIpqCy98"  // your Bot Token (Get from Botfather)

extern float setpoint;
extern int duty;
extern bool machine_on;

void bot_init();
bool bot_connected();
void bot_update();

#endif
