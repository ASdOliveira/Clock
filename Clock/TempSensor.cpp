#include "TempSensor.h"
#include "Defines.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire oneWire(SENSOR_PIN);
static DallasTemperature sensortemp(&oneWire);

// TempSensor::TempSensor(int sensorPin)
// {
//   this->sensorPin = sensorPin;
//   oneWirePtr = new OneWire(sensorPin);
//   sensortempPtr = new DallasTemperature(oneWirePtr);
//   sensortempPtr->begin();
// }

void TempSensor::Init()
{
  sensortemp.begin();
}

float TempSensor::getTemperature()
{
  //Serial.println("GET TEmperature [+]");
  //sensortemp.requestTemperatures();
  //Serial.println("Get tempe {--}");
  //return sensortemp.getTempCByIndex(0);
  return 0.0;
}
