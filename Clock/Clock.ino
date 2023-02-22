#include "OTA.h"
#include "NTPWrapper.h"
#include "Display.h"
#include "Defines.h"
#include "WIFI.h"

Display *display = Display::getInstance();
OTA ota;
NTPWrapper NTPTimer;
WIFI wifi;

void setup()
{
  display->BootMenu();
  delay(2000);

  wifi.Init();
  ota.Initialize();
  NTPTimer.ForceUpdate();
}

void loop() 
{
  wifi.CheckConnection();
  ota.Handle();

  NTPTimer.Update();

  //*********  1st menu, displays: Hour : minuts
 
  display->ShowCurrentTime(NTPTimer.GetHours(), NTPTimer.GetMinutes());

  delay(clockTime);

//*********  2nd menu, displays: Temperature, weekday and the date
  display->ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay());

  delay(tempTime); 
}