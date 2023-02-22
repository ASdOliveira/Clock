#include "Arduino.h"
#include <ArduinoOTA.h>
#include "OTA.h"
#include "Display.h"

Display *lcd_display = Display::getInstance();

OTA::OTA()
{

}

void OTA::Handle()
{
  ArduinoOTA.handle();
}

void OTA::Initialize()
{
  ArduinoOTA.onStart([]() 
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) 
    {
      type = "sketch";
    } 
    else 
    { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()

    lcd_display->StartingOTA();    
    delay(200);
  });

  ArduinoOTA.onEnd([]() 
  {
    lcd_display->EndingOTA();
    delay(100);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {  
    static boolean onlyOnce = false;
    lcd_display->ProgressOTA(String((progress / (total / 100))), onlyOnce);
  });

  ArduinoOTA.onError([](ota_error_t error) 
  {
    lcd_display->ErrorOTA();    
    delay(100);
  });

  ArduinoOTA.begin();
}
