#include "Arduino.h"
#include <ArduinoOTA.h>
#include "OTA.h"

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

    // lcd.clear();
    // lcd.home();
    // lcd.print(F("  Starting OTA  "));
    // delay(200);
  });

  ArduinoOTA.onEnd([]() 
  {
  //   lcd.clear();
  //   lcd.home();
  //   lcd.print(F("    End OTA    "));
  //   delay(100);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {  
  //  static boolean onlyOnce = false;
  //   if(onlyOnce == false)
  //   {
  //     onlyOnce = true;
  //     lcd.clear();
  //     lcd.home();
  //     lcd.print(F(" OTA  progress:"));
  //     delay(100);
  //   }
    
  //   else
  //   {
  //      lcd.setCursor(6,1);
  //      lcd.print(String((progress / (total / 100))) + (char)37); //(char)37 is the same as %
  //   }
  });

  ArduinoOTA.onError([](ota_error_t error) 
  {
    // lcd.clear();
    // lcd.home();
    // lcd.print(F("    ERROR OTA    "));
    // delay(100);
  });

  ArduinoOTA.begin();
}
