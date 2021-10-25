#include <Arduino.h>
#include <libexg.h>

//#define SAMPLE_RATE 125
#define BAUD_RATE 9600
int INPUT_PIN = 10;

LibEXG libEXG(true);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
}

void loop()
{
  // put your main code here, to run repeatedly:
  float EOG_Data = libEXG.get_EOG(INPUT_PIN);

  Serial.println(EOG_Data);
}
