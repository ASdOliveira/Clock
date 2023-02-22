#include <Ticker.h>
#include "OTA.h"
#include "NTPWrapper.h"
#include "Display.h"
#include "Defines.h"
#include "WIFI.h"

Display *display = Display::getInstance();
OTA ota;
NTPWrapper NTPTimer;
WIFI wifi;
Ticker displayFlipper;

float displayTime = 0;
bool isShowingDatetime = true;

void setup()
{
  display->BootMenu();
  delay(2000);

  wifi.Init();
  ota.Initialize();
  NTPTimer.ForceUpdate();
  displayFlipper.attach(UPDATE_SCREEN_TIME ,changeDisplayInfo);
}

void loop() 
{
  wifi.CheckConnection();
  ota.Handle();

  NTPTimer.Update();
}

void changeDisplayInfo()
{
  if(displayTime <= DATETIME_PERIOD)
  {
    display->ShowCurrentTime(NTPTimer.GetHours(), NTPTimer.GetMinutes());
  }
  else if(displayTime > DATETIME_PERIOD && displayTime <= TOTAL_PERIOD)
  {
    display->ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay());
  }
  else
  {
    displayTime = 0;
  }

  displayTime += UPDATE_SCREEN_TIME;
}
