#include "WIFI.h"
#include <WiFiManager.h>
#include "Defines.h"
#include "Display.h"

WiFiManager wifiManager;
Display *m_display = Display::getInstance();

unsigned long lastReconnectAttempt = 0;
bool hasAlreadyConnected = false;

void saveConfigsCallback();

void WIFI::Init() 
{
  m_display->ConnectingToWifi();

  WiFi.mode(WIFI_STA);
  wifiManager.setConfigPortalTimeout(60); //Waiting to connect to an old known Wifi for 60s
  wifiManager.setSaveConfigCallback(saveConfigsCallback);
  wifiManager.setDebugOutput(false);
  
  wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
}

void WIFI::CheckConnection() 
{
 if(WiFi.status() != WL_CONNECTED)
  {
    unsigned long now = millis();
    if((now - lastReconnectAttempt > TIME_TO_WAIT_WIFI_CONNECTION) && hasAlreadyConnected)
    {
      lastReconnectAttempt = now;
      m_display->ConnectingToWifi();
      wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
    }
  }
}

void saveConfigsCallback()
{
  m_display->ConnectedWifi(WiFi.localIP().toString());
  hasAlreadyConnected = true;
}