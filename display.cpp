#include "display.h"
#include "bot.h"
#include "temperature.h"
#include "images.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

long display_last_time;			//last display update

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

static void display_connect_probe() 
{
    display.clearDisplay();
    display.drawBitmap(0, 0, probe, 64, 22, WHITE);
	display.setTextSize(1);
  	display.setTextColor(WHITE);
    display.setCursor(10,25);
    display.print("connect");
	display.setCursor(17,33);
	display.print("probe");
	display.display();
}

static void display_probe_fail() 
{
    display.clearDisplay();
    display.drawBitmap(0, 0, probe, 64, 22, WHITE);
	display.setTextSize(1);
  	display.setTextColor(WHITE);
    display.setCursor(17,25);
	display.print("probe");
	display.setCursor(19,33);
	display.print("fail");
    display.display();
}

void display_init()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
}

void display_bot_logo()
{
	display.clearDisplay();
    //display start up logo
	display.drawBitmap(0, 0, imgCoffee, 64, 48, WHITE);
	display.display();
}

void display_turn_off()
{
	display.clearDisplay();
	display.display();
}

void display_wifi_connect()
{
    display.clearDisplay();
	display.drawBitmap(0, 0, wifi, 64, 22, WHITE);
	display.setTextSize(1);
  	display.setTextColor(WHITE);
  	display.setCursor(10,36);
	display.print("WiFi OK!");
  	display.display();
}

void display_update()
{
	if ((millis() - display_last_time) >= DISP_PERIOD_MS)
	{
		display_last_time = millis();

        if (!temperature_probe_present())
		{
  			display_connect_probe();
			return;
		}

        if (temperature_probe_short())
        {
            display_probe_fail();
            return;
        }
        //print status info
		display.clearDisplay();
		display.setTextSize(2);
		display.setTextColor(WHITE);
		display.setCursor(0,0);
		float num = temperature_read();
		char num1[5];
		dtostrf(num, 5, 1, num1);
		display.println(num1);
		num = setpoint;  
		char num2[5];
		dtostrf(num, 5, 1, num2);
		display.println(num2);
		char num3[3];
		dtostrf(duty, 3, 0, num3);
		display.print(num3);
		display.print(" ");
		display.write(24);//up arrow
		display.println();
		display.display();
	}
}