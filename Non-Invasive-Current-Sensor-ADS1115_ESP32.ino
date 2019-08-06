#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);

void setup() {
    Serial.begin(9600);
    ads.setGain(GAIN_ONE); //       +/- 4.096V  1 bit = 0.125mV
    ads.begin();
}

#define ADCSamples        65536.0 //16 bit ADC
#define OneSampleVolt     0.125 //+/- 4.096V  1 bit = 0.125mV//0.1875F;
#define ZeroCorrection    0
float mVperAmp = 668.0/8.0; //8A corresponds 688 mV

double Voltage = 0;
double Vrms = 0;
double AmpsRMS = 0;
double Vpeak = 0;

uint32_t start_time = 0;
int readValue = 0;
int maxValue = 0;
int minValue = ADCSamples;

void loop() {
  if (millis() - start_time < 250)
  {
    double readValue = ads.readADC_SingleEnded(0); //16 bits ADC read of input A0
    maxValue = (readValue > maxValue) ? readValue : maxValue;
    minValue = (readValue < minValue) ? readValue : minValue;
  }
  else
  {
    Vpeak = (maxValue - minValue)/2 * OneSampleVolt;
    Vrms = Vpeak * 0.707;
    AmpsRMS = Vrms/mVperAmp - ZeroCorrection;
    Serial.println("Amps RMS: " + String(AmpsRMS));
    maxValue = 0;
    minValue = ADCSamples;
    start_time = millis();
  }
}
