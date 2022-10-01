#include "Arduino.h"
#include "LED.h"

LED::LED(int pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void LED::Blink()
{
  Status(LedStatus::ON);
  delay(100);
  Status(LedStatus::OFF);
  delay(100);
}

void LED::Status (LedStatus status)
{
  digitalWrite(pin, (int)status); //Internal led turns ON with Low level, if more led will be needed, this method should be extended
}