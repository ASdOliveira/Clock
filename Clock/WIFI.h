#ifndef __WIFI_ESP_H__
#define __WIFI_ESP_H__

#include <Arduino.h>
#include "Display.h"

class WIFI
{
public:
  WIFI(Display &display);
  void Init();
  void CheckConnection();
};


#endif