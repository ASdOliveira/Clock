#ifndef __TEMPSENSOR_H__
#define __TEMPSENSOR_H__

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class TempSensor
{
  private:
  int sensorPin;
  OneWire *oneWirePtr;
  DallasTemperature *sensortempPtr;

  public:
  TempSensor(int sensorPin);
  float getTemperature();
};

#endif