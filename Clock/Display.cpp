#include "Display.h"
#include <LiquidCrystal.h> 
#include <BigFont02.h>
#include "TempSensor.h"
#include "Defines.h"

LiquidCrystal lcd(D6, D5, D3, D2, D1, D7);
BigFont02     big(&lcd);
TempSensor temperatureSensor(SENSOR_PIN);
static Display *instancePtr;
  
Display::Display()
{
  lcd.begin(16,2); 
  big.begin();
}

Display *Display::getInstance()
{
  if (instancePtr == NULL)
    {
      instancePtr = new Display();
      return instancePtr;
    }
    else
    {
      return instancePtr;
    }
}

void Display::BootMenu()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("     Hello !!   ");
  lcd.setCursor(0,1);
  lcd.print("    Booting...  ");
}

void Display::ConnectedWifi(String localIP)
{
  lcd.setCursor(1,0);
  lcd.print("WiFi connected");
  lcd.setCursor(1,1);
  lcd.print(localIP);
}

void Display::ShowCurrentTime(int hour, int minutes)
{
  lcd.clear();
  lcd.home();

  big.writeint(0,0, hour,2,false); 
  big.writeint(0,9, minutes,2,true);

 //if condition to handle the two dots position

  if(((int) minutes >= 10) && ((int) minutes < 20))
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
}

void Display::ShowTemperatureAndDate(String date, String day)
{
  int temperature = (int)temperatureSensor.getTemperature();
  
  lcd.clear();
  lcd.home();
  
  big.writeint(0,0,temperature,2,false);
 
  lcd.setCursor(10,0);
  lcd.print(date); //Receive these information from parameter
  lcd.setCursor(6,1);
  lcd.print((char)223);
  lcd.setCursor(7,1);
  lcd.print("C");
  lcd.setCursor(11,1);
  lcd.print(day);
}

void Display::ConnectingToWifi()
{
  lcd.clear();
  lcd.home();
  lcd.print("  Waiting you  ");
  lcd.setCursor(0, 1);
  lcd.print("show me a WiFi");
}