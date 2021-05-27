#include "bot.h"
#include "temperature.h""
#include "pid.h"
#include "display.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "arduino_secrets.h"

// Initialize Wifi connection to the router
char ssid[] = SECRET_SSID;     // your network SSID (name)
char password[] = SECRET_PSW; // your network key

//need fingerprint to connect to telegram server (api.telegram.org)
// https://www.grc.com/fingerprints.htm

const uint8_t fingerprint[20] = { 
	0xF2, 0xAD, 0x29, 0x9C, 0x34, 
	0x48, 0xDD, 0x8D, 0xF4, 0xCF, 
	0x52, 0x32, 0xF6, 0x57, 0x33, 
	0x68, 0x2E, 0x81, 0xC1, 0x90
};

WiFiClientSecure client;

UniversalTelegramBot bot(SECRET_BOT_TOKEN, client);

String last_text;
String last_chat_id;
long bot_last_time;             //last time messages scan has been done

static void handleNewMessages(int numNewMessages)
{
	Serial.println("handleNewMessages");
	Serial.println(String(numNewMessages));
	
	for (int i=0; i<numNewMessages; i++)
	{
    	String chat_id = String(bot.messages[i].chat_id);
    	String text = bot.messages[i].text;
    	String from_name = bot.messages[i].from_name;
    	if (from_name == "") from_name = "Guest";

 		if ((text == "/on"))
		{
      		String msg = "Coffee machine turned on!\n";
			machine_on = true;
			display_bot_logo();
      		bot.sendMessage(chat_id, msg);
			delay(500);
    	}

		if ((text == "/off"))
		{
      		String msg = "Coffee machine turned off!\n";
			machine_on = false;
			display_turn_off();
      		bot.sendMessage(chat_id, msg);
    	}

		if ((text == "/target"))
		{
      		String msg;
			msg += "Current target is: ";
			msg += setpoint;
			msg += "°C";
			msg += "\n";
			msg += "";
			msg += "Send me the new temperature, please use this format:\n\n";
			msg += "90 or 90.5:\n\n";
			msg += "/cancel\n";
      		bot.sendMessage(chat_id, msg);
    	}

		if (text == "/readtemp")
		{
			String msg = "Temperature 1 is ";
			msg += String(temperature_read(), 1) ;
			msg += "\n";
			bot.sendMessage(chat_id, msg, "");
		}

		if (text == "/status")
		{ 
			String msg = "Setpoint: ";
			msg += String(setpoint, 1);
			msg += "°C\n";
			msg += "Temperature: ";
			msg += String(temperature_read(), 1);
			msg += "°C\n";
			msg += "Output: ";
			msg += duty;
			msg += "%\n";
			msg += "P: ";
			msg += pid_get_kp();
			msg += "\n";
			msg += "I: ";
			msg += pid_get_ki();
			msg += "\n";
			msg += "D: ";
			msg += pid_get_kd();
			msg += "\n";
			bot.sendMessage(chat_id, msg, "");
		}

		if ((text == "/pid"))
		{
      		String keyboardJson = "[[\"/p\", \"/i\"],[\"/d\"]]";
			String msg;
			msg += "Current PID is:\n";
			msg += "P: ";
			msg += pid_get_kp();
			msg += "\n";
			msg += "I: ";
			msg += pid_get_ki();
			msg += "\n";
			msg += "D: ";
			msg += pid_get_kd();
			msg += "\n";
			msg += "Which PID parameter you want to set?:\n\n";
			msg += "/cancel\n";
      		bot.sendMessageWithReplyKeyboard(chat_id, msg, "", keyboardJson, true, true);
    	}

		if ((text == "/p"))
		{
			String msg;
			msg += "Ok. Send me the new P:\n\n";
			msg += "/cancel\n";
      		bot.sendMessage(chat_id, msg);
    	}

		if ((text == "/i"))
		{
			String msg;
			msg += "Ok. Send me the new I:\n\n";
			msg += "/cancel\n";
      		bot.sendMessage(chat_id, msg);
    	}

		if ((text == "/d"))
		{
			String msg;
			msg += "Ok. Send me the new D:\n\n";
			msg += "/cancel\n";
      		bot.sendMessage(chat_id, msg);
    	}

		if (text == "/start")
		{
			String welcome = "Welcome to CoffeBot, " + from_name + ".\n";
			String keyboardJson = "[[\"/on\", \"/off\"],[\"/target\", \"/readtemp\"],[\"/status\", \"/pid\"]]";
			welcome += "This is what you can do:\n\n";
			welcome += "/on : turn on the coffee machine\n";
			welcome += "/off : turn off the coffee machine\n";
			welcome += "/target : set target temperature\n";
			welcome += "/readtemp : read current temperature\n";
			welcome += "/status : get status\n";
			welcome += "/pid : set PID parameters\n";
			bot.sendMessageWithReplyKeyboard(chat_id, welcome, "", keyboardJson, true);
		}

		if(text == "/cancel" )
		{
			bot.sendMessage(bot.messages[i].chat_id, "Ok. Set parameter canceled\n", "");
		}

		//check if request is from the same user
		if(last_chat_id == chat_id)
		{
			if (last_text == "/target")
			{
				float new_setpoint = text.toFloat();
				//check if value is ok
				if ((new_setpoint <= 100) && (new_setpoint > 0))
				{
					setpoint = new_setpoint;
					Serial.println("Ok! Target set to " + String(setpoint, 1));
					bot.sendMessage(bot.messages[i].chat_id, ("Ok! Target set to " + String(setpoint, 1) + "°C\n"), "");
				}
				else
				{
					bot.sendMessage(bot.messages[i].chat_id, "Target out of range\n", "");
				}
			}
			else if (last_text == "/p")
			{
				int new_p = text.toInt();
				if (new_p >= 0)
				{
					pid_set_kp(new_p);
					Serial.println("Ok! P set to " + String(pid_get_kp()));
					bot.sendMessage(bot.messages[i].chat_id, ("Ok! P set to " + String(pid_get_kp()) + "\n"), "");
				}
				else
				{
					bot.sendMessage(bot.messages[i].chat_id, "Parameter out of range\n", "");
				}
			}
			else if (last_text == "/i")
			{
				int new_i = text.toInt();
				if (new_i >= 0)
				{
					pid_set_ki(new_i);
					Serial.println("Ok! I set to " + String(pid_get_ki()));
					bot.sendMessage(bot.messages[i].chat_id, ("Ok! I set to " + String(pid_get_ki()) + "\n"), "");
				}
				else
				{
					bot.sendMessage(bot.messages[i].chat_id, "Parameter out of range\n", "");
				}
			}
			else if (last_text == "/d")
			{
				int new_d = text.toInt();
				if (new_d >= 0)
				{
					pid_set_kd(new_d);
					Serial.println("Ok! D set to " + String(pid_get_kd()));
					bot.sendMessage(bot.messages[i].chat_id, ("Ok! D set to " + String(pid_get_kd()) + "\n"), "");
				}
				else
				{
					bot.sendMessage(bot.messages[i].chat_id, "Parameter out of range\n", "");
				}
			}
		}

		last_text = text;
		last_chat_id = chat_id;
	}
}

void bot_init() {
	client.setFingerprint(fingerprint);

    // Set WiFi to station mode and disconnect from an AP if it was Previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

    // attempt to connect to Wifi network:
	Serial.print("Connecting Wifi: ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
}

bool bot_connected()
{
    if (WiFi.status() == WL_CONNECTED)
        return true;
    return false;
}

void bot_update() {
    if (millis() - bot_last_time  > BOT_UPDATE_PERIOD)
	{
		bot_last_time = millis();
		int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
		Serial.println(numNewMessages);
		while(numNewMessages)
		{
			Serial.println("got response");
			handleNewMessages(numNewMessages);
			numNewMessages = bot.getUpdates(bot.last_message_received + 1);
		}
		bot_last_time = millis();
	}
}