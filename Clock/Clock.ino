#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal.h> 
#include <BigFont02.h>
#include <Wire.h>
#include <time.h>
#include <ESP8266mDNS.h>

//new classes:
#include "OTA.h"
#include "LED.h"
#include "TempSensor.h"
/**********************************************************************
*
* Defines
*
**********************************************************************/
#define clockTime 3500   // number of microseconds showing the clock
#define tempTime 2500    // number of microseconds showing the Temp
#define UpdateTime 30000 // Time in microseconds to update date and time
#define SENSOR_PIN D4

#define LED_PIN D0
/**********************************************************************
*
* Global variables
*
**********************************************************************/
const char *ssid     = "CASA";
const char *password = "garantia";
const long utcOffsetInSeconds = -(3600*3);
char daysOfTheWeek[7][8] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
byte updateCounter = 0;
/**********************************************************************
*
* Object instances
*
**********************************************************************/
LiquidCrystal lcd(D6, D5, D3, D2, D1, D7);
BigFont02     big(&lcd);; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, UpdateTime);

OTA ota;
LED led(LED_PIN);
TempSensor temperatureSensor(SENSOR_PIN);
/**********************************************************************
*
* Function prototypes
*
**********************************************************************/
void bootMenu();
float getTemp();
void conectaWiFi();
String getDateFromEpoch(unsigned int epoch);
void connectedWifiMenu(); 
void customDelay();
/**********************************************************************
*
* Setup function
*
**********************************************************************/
void setup()
{
  lcd.begin(16,2); 
  big.begin(); 
  lcd.clear(); 
  
  bootMenu(); //Start up menu
  
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  conectaWiFi(); //Connect Wifi
  
  connectedWifiMenu();
  
  timeClient.begin(); // Connect to NTP server
  if(timeClient.forceUpdate() == true) // Force to update the date
  {
    led.Blink();
    led.Blink();
  }

  ota.Initialize();
}
/**********************************************************************
*
* Loop function
*
**********************************************************************/
void loop() 
{
  conectaWiFi();

  ota.Handle();

  if(timeClient.update() == true)
  {
    led.Blink();
    updateCounter = 0;
  }
  else
  {
    updateCounter++;
    if (updateCounter >= 6) //wait 60 seconds to update..
    {
      if(timeClient.forceUpdate() == true)
      {
        updateCounter = 0;
        led.Blink();
        led.Blink();
      }
    }
  }

  lcd.clear();
  lcd.home();

  //*********  1st menu, displays: Hour : minuts
 
  big.writeint(0,0,timeClient.getHours(),2,false);
  big.writeint(0,9,timeClient.getMinutes(),2,true);

 //if condition to handle the two dots position

  if(((int) timeClient.getMinutes() >= 10) && ((int) timeClient.getMinutes() < 20))
  {
    lcd.setCursor(8,0);
    lcd.print((char)165);  //(char)165 is equal to a dot centered
    lcd.setCursor(8,1);
    lcd.print((char)165);
  }

  else
  {   
    lcd.setCursor(7,0);
    lcd.print((char)165);
    lcd.setCursor(7,1);
    lcd.print((char)165);
  }

  delay(clockTime);

//*********  2nd menu, displays: Temperature, weekday and the date

  int temperature = (int)temperatureSensor.getTemperature();
  
  lcd.clear();
  lcd.home();
  
  big.writeint(0,0,temperature,2,false);
 
  lcd.setCursor(10,0);
  lcd.print((String)getDateFromEpoch(timeClient.getEpochTime()));
  lcd.setCursor(6,1);
  lcd.print((char)223);
  lcd.setCursor(7,1);
  lcd.print("C");
  lcd.setCursor(11,1);
  lcd.print((String) daysOfTheWeek[timeClient.getDay()]);

  delay(tempTime); 
}

/**********************************************************************
*
* bootMenu() - function that prints a text while is booting  
*
**********************************************************************/

void bootMenu()
{
  lcd.setCursor(0,0);
  lcd.print(F("     Hello !!   "));
  lcd.setCursor(0,1);
  lcd.print(F("    Booting...  "));
  delay(2000);
  lcd.clear();
}
/**********************************************************************
*
* show the connected IP address
*
**********************************************************************/
void connectedWifiMenu()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(F("WiFi connected"));
  lcd.setCursor(1,1);
  lcd.print(WiFi.localIP());
  delay(2000);
}
/**********************************************************************
*
* conectaWiFi() - function which handles the wifi connection 
*
**********************************************************************/
void conectaWiFi() 
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
      lcd.home();
      lcd.print(F("WiFi connecting"));
      while (WiFi.status() != WL_CONNECTED) 
      {
        lcd.setCursor(i,1);
        lcd.print((char)255);
        i++;
        if(i>16)
        {
          i=0;
          lcd.clear();
          lcd.print(F("WiFi connecting"));
        }
        
        led.Blink();
        
        count++;
        if(count >= 100)
        {
          lcd.clear();
          lcd.home();
          lcd.print(F("  Reseting ...  "));
          delay(200);
          ESP.restart();
        }
      }
      timeClient.begin(); // Connect to NTP server
  }
}

/**********************************************************************
*
* getDateFromEpoch - converts the epoch time to the Date and return in a string 
*
**********************************************************************/
String getDateFromEpoch(unsigned long epoch)
{
    time_t rawtime = epoch;
    struct tm  ts;
    String DateString = "", MonthString = "";

    ts = *localtime(&rawtime);

    switch(ts.tm_mon)
  {
    case 0: MonthString = "Jan";
    break;
    case 1: MonthString = "Fev";
    break;
    case 2: MonthString = "Mar";
    break;
    case 3: MonthString = "Abr";
    break;
    case 4: MonthString = "Mai";
    break;
    case 5: MonthString = "Jun";
    break;
    case 6: MonthString = "Jul";
    break;
    case 7: MonthString = "Ago";
    break;
    case 8: MonthString = "Set";
    break;
    case 9: MonthString = "Out";
    break;
    case 10: MonthString = "Nov";
    break;
    case 11: MonthString = "Dez";
    break;
  }
  DateString = (String) ts.tm_mday + "/";
  DateString += (String) MonthString;

  return DateString;
}
