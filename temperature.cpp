#include "temperature.h"
#include "src/Adafruit_ADS1X15/Adafruit_ADS1X15.h"

uint16_t samples[SAMPLE_NUM];
int sample_i = 0;
float savedTemp = 0.0;
int savedAverageADC = 0;
long temperature_last_time;
Adafruit_ADS1115 ads;
bool probe_present;
bool probe_short;

void temperature_init()
{
    ads.begin();
    ads.setGain(GAIN_ONE);
    ads.startADC_Cont(ADC_CHANNEL);
    delay(250);
    int sample = ads.getLastConversionResults();
    if (sample < 25500)
    {
        probe_present = true;
    }
    float x = ads.computeVolts(sample);  //measured analog voltage [V]
    x = R_SERIES * (x / (float(VREF) / 1000 - x));  // measured Rntc value [ohm]
    x = (1 / (1 / (NTC_TEMP_AMB + 273.15) + log(x / NTC_RES_AMB) / BETA)) - 273.15;  //measured temp ['C]          
    savedTemp = x < 0 ? 0 : x;
}

void temperature_update()
{
  //read temp loop here
  if (millis() - temperature_last_time > SAMPLE_PERIOD_MS)
  {
      temperature_last_time = millis();
      samples[sample_i] = ads.getLastConversionResults();
      
      sample_i++;
      if (sample_i >= SAMPLE_NUM)
      {
          sample_i =0;
          long averageSample = 0;
          for (int i=0; i < SAMPLE_NUM; i++)
          {
              averageSample += samples[i];
          }
          averageSample /= SAMPLE_NUM;
          Serial.print("average sample: ");
          Serial.println(averageSample);
          savedAverageADC = averageSample;
          //convert raw to temperature in Celsius
          Serial.print("savedAverageADC: ");
          Serial.println(savedAverageADC);
          if (savedAverageADC > 25500)
          {
              savedTemp = 0;
              probe_present = false;
              return;
          }
          if (savedAverageADC < 100)
          {
              savedTemp = 250;
              probe_short = true;
              return;
          }
          probe_present = true;
          probe_short = false;
          
          float x = ads.computeVolts(averageSample);  //measured analog voltage [V]
          Serial.print("V: ");
          Serial.println(x);
          x = R_SERIES * (x / (float(VREF) / 1000 - x));  // measured Rntc value [ohm]
          Serial.print("R: ");
          Serial.println(x);
          x = (1 / (1 / (NTC_TEMP_AMB + 273.15) + log(x / NTC_RES_AMB) / BETA)) - 273.15;  //measured temp ['C]          
          Serial.print("T: ");
          Serial.println(x);
          savedTemp = x < 0 ? 0 : x;
      }
  }
}

float temperature_read()
{
    return savedTemp;
}

int temperature_raw()
{
    return savedAverageADC;
}

bool temperature_probe_present()
{
  return probe_present;
}

bool temperature_probe_short()
{
  return probe_short;
}