#ifndef __LED_H__
#define __LED_H__

enum class LedStatus {ON, OFF};

class LED
{
  private:
  int pin;

  public:
  LED(int pin);
  void Blink();
  void Status (LedStatus status);
};

#endif
