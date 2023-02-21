#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

class Display
{
  public:
  Display();
  void BootMenu();
  void ConnectedWifi(String localIP);
  void ShowCurrentTime(int hour, int minutes);
  void ShowTemperatureAndDate(String date, String day);
  void ConnectingToWifi();
  void LoadingAnimation(byte position);
  void Reseting();
};

#endif