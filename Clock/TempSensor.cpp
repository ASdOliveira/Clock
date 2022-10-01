#include "TempSensor.h"

TempSensor::TempSensor(int sensorPin)
{
  this->sensorPin = sensorPin;
  oneWirePtr = new OneWire(sensorPin);
  sensortempPtr = new DallasTemperature(oneWirePtr);
  sensortempPtr->begin();
}

float TempSensor::getTemperature()
{
  sensortempPtr->requestTemperatures();
  return sensortempPtr->getTempCByIndex(0);
}
