#ifndef __TEMPSENSOR_H__
#define __TEMPSENSOR_H__

#include "Arduino.h"


class TempSensor
{
  private:
  // int sensorPin;
  // OneWire *oneWirePtr;
  // DallasTemperature *sensortempPtr;

  public:
  //TempSensor(int sensorPin);
  void Init();
  float getTemperature();
};

#endif