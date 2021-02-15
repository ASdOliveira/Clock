/**********************************************************************
*
* Clock using NTP server, esp8266 and a 16x2 LCD display 
*
**********************************************************************/

/**********************************************************************
*
* Library Includes
*
**********************************************************************/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal.h> 
#include <BigFont02.h>
#include <Wire.h>
#include <time.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>
/**********************************************************************
*
* Defines
*
**********************************************************************/
#define clockTime 3500   // number of microseconds showing the clock
#define tempTime 2500    // number of microseconds showing the Temp
#define UpdateTime 30000 // Time in microseconds to update date and time
#define SENSOR_PIN D4
#define LED_HIGH LOW     //state in which the LED turns off
#define LED_LOW HIGH
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
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensortemp(&oneWire);
/**********************************************************************
*
* Function prototypes
*
**********************************************************************/
void bootMenu();
float getTemp();
void conectaWiFi();
String getDateFromEpoch(unsigned int epoch);
void blinkLed();
void connectedWifiMenu(); 
void customDelay();
/**********************************************************************
*
* Setup function
*
**********************************************************************/
void setup()
{
  pinMode(LED_PIN,OUTPUT);
  
  lcd.begin(16,2); 
  big.begin(); 
  lcd.clear(); 
  
  bootMenu(); //Start up menu

  sensortemp.begin(); 
  
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  conectaWiFi(); //Connect Wifi
  
  connectedWifiMenu();
  
  timeClient.begin(); // Connect to NTP server
  if(timeClient.forceUpdate() == true) // Force to update the date
  {
    blinkLed();
    blinkLed();
  }

//*************** OTA CALLBACKS *****************

  ArduinoOTA.onStart([]() {
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
 
    lcd.clear();
    lcd.home();
    lcd.print(F("  Starting OTA  "));
    delay(200);
  });

  ArduinoOTA.onEnd([]() {

    lcd.clear();
    lcd.home();
    lcd.print(F("    End OTA    "));
    delay(100);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    
   static boolean onlyOnce = false;
    if(onlyOnce == false)
    {
      onlyOnce = true;
      lcd.clear();
      lcd.home();
      lcd.print(F(" OTA  progress:"));
      delay(100);
    }
    
    else
    {
       lcd.setCursor(6,1);
       lcd.print(String((progress / (total / 100))) + (char)37); //(char)37 is the same as %
    
    }

    
  });

  ArduinoOTA.onError([](ota_error_t error) {
    lcd.clear();
    lcd.home();
    lcd.print(F("    ERROR OTA    "));
    delay(100);
  });

  ArduinoOTA.begin();
}
/**********************************************************************
*
* Loop function
*
**********************************************************************/
void loop() 
{
  ArduinoOTA.handle();
  
  conectaWiFi();
  
  if(timeClient.update() == true)
  {
    blinkLed();
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
        blinkLed();
        blinkLed();
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

  int temperature = (int)getTemp();
  
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
* getTemp() - function that returns the read temperature  
*
**********************************************************************/
float getTemp()
{
  sensortemp.requestTemperatures();
  float TempLida = sensortemp.getTempCByIndex(0);

  return TempLida;
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
     digitalWrite(LED_PIN,LED_LOW); //LED turns off with HIGH level
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
        
        blinkLed();
        
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
* blinkLed() 
*
**********************************************************************/
void blinkLed()
{
        digitalWrite(LED_PIN,LED_HIGH);
        delay(100);
        digitalWrite(LED_PIN,LED_LOW);
        delay(100);
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
