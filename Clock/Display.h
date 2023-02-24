#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

class Display
{ 
  public:
  Display();
  void begin();
  void BootMenu();
  void ConnectedWifi(String localIP);
  void ShowCurrentTime(int hour, int minutes);
  void ShowTemperatureAndDate(String date, String day, int temperature);
  void ConnectingToWifi();
  void FetchingUpdatedTime();
  void StartingOTA();
  void EndingOTA();
  void ErrorOTA();
  void ProgressOTA(String progress, bool &flag);
};

#endif