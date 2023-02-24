#include "OTA.h"
#include "NTPWrapper.h"
#include "Display.h"
#include "Defines.h"
#include "WIFI.h"
#include <OneWire.h>
#include <DallasTemperature.h>

Display display;
OTA ota(display);
WIFI wifi(display);
NTPWrapper NTPTimer;
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensortemp(&oneWire);

unsigned long now = 0;
unsigned long lastDelay = 0;
unsigned long diffTime = 0;
int temperature = 0;
bool isShowingHour = true;
bool isShowingTemperature = false;

void setup()
{
  display.begin();
  display.BootMenu();
  delay(2000);
  
  sensortemp.begin();
  wifi.Init();
  ota.Initialize();
  
  display.FetchingUpdatedTime();
  delay(2000);
  NTPTimer.ForceUpdate();
  
  lastDelay = millis();
}

void loop() 
{
  now = millis();
  diffTime = now - lastDelay;

  if(diffTime <= DATETIME_PERIOD && isShowingHour == true)
  {
    NTPTimer.Update();
    display.ShowCurrentTime(NTPTimer.GetHours(), NTPTimer.GetMinutes());
    isShowingHour = false;
  }
  else if(diffTime > DATETIME_PERIOD && diffTime <= TOTAL_PERIOD && isShowingHour == false)
  {
    temperature = (int)getTemp();
    display.ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay(), temperature);
    
    isShowingHour = true;
    isShowingTemperature = true;
  }
  else if (diffTime > TOTAL_PERIOD && isShowingTemperature)
  {
    lastDelay = now;
  }

  wifi.CheckConnection();
  ota.Handle();

  yield();
}

float getTemp()
{
  sensortemp.requestTemperatures();
  float TempLida = sensortemp.getTempCByIndex(0);

  return TempLida;
}
