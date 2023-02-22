#ifndef __OTA_H__
#define __OTA_H__

#include "Arduino.h"
#include "Display.h"

class OTA
{
public:
  OTA(Display &display);
  void Handle();
  void Initialize();
};

#endif