#ifndef temperature_h
#define temperature_h

#include "src/Adafruit_ADS1X15/Adafruit_ADS1X15.h"

#define ADC_CHANNEL 0
#define SAMPLE_PERIOD_MS 50
#define SAMPLE_NUM 5
#define NTC_TEMP_AMB 25
#define VREF 3300  //wemos d1 mini vref
#define NTC_RES_AMB 49926
#define R_SERIES 11990
#define BETA 4030

void temperature_init();
void temperature_update();
float temperature_read();
int temperature_raw();
bool temperature_probe_present();
bool temperature_probe_short();

#endif 