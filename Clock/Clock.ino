#include "OTA.h"
#include "LED.h"
#include "NTPWrapper.h"
#include "Display.h"
#include "Defines.h"
#include "WIFI.h"

Display display;
OTA ota;
LED led(LED_PIN);
NTPWrapper NTPTimer;
WIFI wifi;

/**********************************************************************
*
* Setup function
*
**********************************************************************/
void setup()
{
  display.BootMenu();
  delay(2000);

  wifi.Init();  // Will wait until finish the config
  
  //display.ConnectedWifi(WiFi.localIP().toString());
  //delay(2000);

  ota.Initialize();
}
/**********************************************************************
*
* Loop function
*
**********************************************************************/
void loop() 
{
  wifi.CheckConnection();

  ota.Handle();

  NTPTimer.Update();

  //*********  1st menu, displays: Hour : minuts
 
  display.ShowCurrentTime(NTPTimer.GetHours(), NTPTimer.GetMinutes());

  delay(clockTime);

//*********  2nd menu, displays: Temperature, weekday and the date
  display.ShowTemperatureAndDate(NTPTimer.GetDate(), NTPTimer.GetDay());

  delay(tempTime); 
}

/**********************************************************************
*
* conectaWiFi() - function which handles the wifi connection 
*
**********************************************************************/
/*void conectaWiFi() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    led.Status(LedStatus::OFF);
  }
  else
  {
      WiFi.disconnect();
      WiFi.begin(ssid, password); 
  
      byte i = 0;
      byte count = 0;
      display.ConnectingToWifi();

      while (WiFi.status() != WL_CONNECTED) 
      {
        display.LoadingAnimation(i);      
        i++;
        
        if(i>16)
        {
          i=0;
          display.ConnectingToWifi();
        }
        
        led.Blink();
        
        count++;
        if(count >= 100)
        {
          display.Reseting();
          delay(200);
          ESP.restart();
        }
      }
      
      NTPTimer.Update(); //Check if this update is indeed needed
  }
}*/