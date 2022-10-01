#ifndef __NTPWrapper_H__
#define __NTPWrapper_H__
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Arduino.h"

class NTPWrapper
{
  private:
  int updateCounter = 0;
  String getDateFromEpoch(unsigned long epoch);

  public:
  NTPWrapper();
  void Update();
  int GetHours();
  int GetMinutes();
  String GetDay();
  String GetDate();
};

#endif