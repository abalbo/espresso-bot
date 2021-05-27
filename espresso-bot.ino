#include "temperature.h"
#include "bot.h"
#include "display.h"
#include "pid.h"
#include <Arduino.h>

#define PIN_RESET 255  //
#define DC_JUMPER 0    // I2C Addres: 0 - 0x3C, 1 - 0x3D
#define RELAY_OUT D5

extern bool machine_on = true;
extern float setpoint = 90;
int duty;

#define PWM_PERIOD 5000      //PWM uperiod in ms
long pwm_last_time = 0;      //last pwm period
uint16_t turn_on;            //heater turn on time in ms

void setup(void)
{
	pinMode(RELAY_OUT, OUTPUT);
	digitalWrite(RELAY_OUT, LOW);
	Serial.begin(115200);
	display_init();
	display_bot_logo();
	delay(1000);
	temperature_init();
	bot_init();

	while (!bot_connected)
	{
		Serial.print(".");
	  	delay(500);
	}
	Serial.println("WiFi connected");
	display_wifi_connect();
	delay(1000);
}

void loop (void)
{
	bot_update();
	if (machine_on)
	{
		temperature_update();
		display_update();
		pwm_update();
	}
	else
	{
		digitalWrite(RELAY_OUT, LOW);
	}
}

void pwm_update()
{
    if ((millis() - pwm_last_time) >= PWM_PERIOD)
    {
		//compute new ON time for heater
    	pwm_last_time = millis();
		duty = pid_update(setpoint, temperature_read());
		duty = duty > 90 ? 100 : duty;
    	duty = duty < 10 ? 0 : duty;
		turn_on = PWM_PERIOD * duty / 100;
		Serial.print("turn on time: ");
        Serial.println(turn_on);
    }

    if ((millis() - pwm_last_time) < turn_on)
    {
        digitalWrite(RELAY_OUT, HIGH);
    }
    else if ((millis() - pwm_last_time) > turn_on)
    {
    	digitalWrite(RELAY_OUT, LOW); 
    }
    
}
