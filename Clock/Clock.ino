#include <Ticker.h>
#include "OTA.h"
#include "NTPWrapper.h"
#include "Display.h"
#include "Defines.h"
#include "WIFI.h"

Display display;
OTA ota(display);
WIFI wifi(display);
NTPWrapper NTPTimer;
Ticker displayFlipper;

float displayTime = 0;
bool updateHour = true;
bool updateTemperature = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup [+]");
  display.begin();
  display.BootMenu();
  delay(2000);
  Serial.println("WIFI");
  wifi.Init();
  Serial.println("OTA");
  ota.Initialize();
  Serial.println("NTP");
  display.FetchingUpdatedTime();
  NTPTimer.ForceUpdate();
  displayFlipper.attach(UPDATE_SCREEN_TIME ,changeDisplayInfo);

  Serial.println("Setup [-]");
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
    //display.ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay());
    display.ShowCurrentTime(NTPTimer.GetHours(), NTPTimer.GetMinutes());
  }
  else if(displayTime > DATETIME_PERIOD && displayTime <= TOTAL_PERIOD)
  {
    display.ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay());
  }
  else
  {
    displayTime = 0;
  }

  displayTime += UPDATE_SCREEN_TIME;
}
