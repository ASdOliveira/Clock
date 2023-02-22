#include "WIFI.h"
#include <WiFiManager.h>
#include "Defines.h"
#include "Display.h"

WiFiManager wifiManager;
Display *m_display = Display::getInstance();

void saveConfigsCallback();

unsigned long lastReconnectAttempt = 0;
bool hasAlreadyConnected = false;

void WIFI::Init() 
{
  WiFi.mode(WIFI_STA);

  m_display->ConnectingToWifi();
 
  //wifiManager.setConfigPortalBlocking(false); // Set to true
  wifiManager.setConfigPortalTimeout(60); //Waiting to connect to an old known Wifi for 60s
  wifiManager.setSaveConfigCallback(saveConfigsCallback);
  wifiManager.setDebugOutput(false);
  
  wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
}

void WIFI::CheckConnection() 
{
  //wifiManager.process();

 if(WiFi.status() != WL_CONNECTED)
  {
    unsigned long now = millis();
    if((now - lastReconnectAttempt > 3000) && hasAlreadyConnected)
    {
      lastReconnectAttempt = now;
      //Serial.println("[WIFI] Reconnecting to wifi, please wait ");
      wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
    }
  }
}

void saveConfigsCallback()
{
  m_display->ConnectedWifi(WiFi.localIP().toString());
  hasAlreadyConnected = true;
}