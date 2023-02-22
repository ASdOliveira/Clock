#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

class Display
{
  private:
  Display();
  
  public:
  Display(const Display &obj) = delete;
  static Display *getInstance();
  void BootMenu();
  void ConnectedWifi(String localIP);
  void ShowCurrentTime(int hour, int minutes);
  void ShowTemperatureAndDate(String date, String day);
  void ConnectingToWifi();
};

#endif