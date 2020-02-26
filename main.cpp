#include <Arduino.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h> 
#include <BigNumbers_I2C.h>

#define clockTime 7000 // number of seconds showing the clock
#define tempTime 2000 // number of seconds showing the Temp

LiquidCrystal_I2C lcd(0x27,16,2); // construct LCD object
BigNumbers_I2C bigNum(&lcd); // construct BigNumbers_I2C object, passing to it the name of our LCD object

const char *ssid     = "casa";
const char *password = "garantia";

const long utcOffsetInSeconds = -(3600*3);

char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void bootMenu();
int getTemp();
void conectaWiFi();
String getDateFromEpoch(unsigned int epoch);

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  lcd.begin(); // setup LCD rows and columns
  bigNum.begin(); // set up BigNumbers
  lcd.clear(); // clear display
  Serial.begin(115200);

  bootMenu(); //Start up menu

  conectaWiFi(); //Connect Wifi
  timeClient.begin(); // Connect to NTP server
  timeClient.forceUpdate(); // Force to update the date
}


byte x = 0;//x & y determines position of character on screen Big Numbers library
byte y = 0;

void loop() 
{
  conectaWiFi();
  timeClient.update();
  lcd.clear();
  
  bigNum.displayLargeInt((int) timeClient.getHours(), 0, 0, 2,false);
  bigNum.displayLargeInt((int) timeClient.getMinutes(), 9, 0, 2,true);
  //Serial.println(getDateFromEpoch(timeClient.getEpochTime()));

  if(((int) timeClient.getMinutes() >= 10) && ((int) timeClient.getMinutes() < 20))
  {
    lcd.setCursor(8,0);
    lcd.print("o");
    lcd.setCursor(8,1);
    lcd.print("o");
  }

  else
  {   
    lcd.setCursor(7,0);
    lcd.print("o");
    lcd.setCursor(7,1);
    lcd.print("o");   
  }

  delay(clockTime);
  
  lcd.clear();
  bigNum.displayLargeInt(getTemp(), 0, 0, 2,false);
  lcd.setCursor(6,0);
  lcd.print("o");
  lcd.setCursor(7,1);
  lcd.print("C");
  lcd.setCursor(11,1);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(9,0);
  lcd.print(getDateFromEpoch(timeClient.getEpochTime()));
  
  delay(tempTime);
}

//Get current temperature
int getTemp()
{
  int temp = 0, valorMedido=0;
  valorMedido = analogRead(A0);
  float milivolts = (valorMedido/1024.0) * 3300;
  temp = (int) milivolts/10;

  return temp;    
}

//start up menu
void bootMenu()
{
  lcd.setCursor(4,0);
  lcd.print("Hello!");
  lcd.setCursor(1,1);
  lcd.print("Powering up...");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(7,0);
  lcd.print("By:");
  lcd.setCursor(0,1);
  lcd.print("Arysson Oliveira");
  delay(1000);
  lcd.clear();
}

//Connect to Wifi
void conectaWiFi() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
     digitalWrite(LED_BUILTIN,HIGH); //LED turns off with HIGH level
     Serial.println("WIFI CONECTADO!!");
  }
  else
  {
      WiFi.begin(ssid, password); // Conecta na rede WI-FI  

       byte i = 0;
      lcd.home();
      lcd.print("WiFi connecting");
      while (WiFi.status() != WL_CONNECTED) 
      {
        lcd.setCursor(i,1);
        lcd.write(0);
        i++;
        if(i>16)
        {
          i=0;
          lcd.clear();
          lcd.print("WiFi connecting");
        }
        
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN,LOW);
        delay(50);
        Serial.print(".");
      }
  }
  
}

String getDateFromEpoch(unsigned int epoch)
{

  static unsigned char month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  static unsigned char week_days[7] = {4,5,6,0,1,2,3};
  //Thu=4, Fri=5, Sat=6, Sun=0, Mon=1, Tue=2, Wed=3

  unsigned char ntp_hour, ntp_minute, ntp_second, ntp_week_day, ntp_date, ntp_month, leap_days, leap_year_ind ;
  unsigned short temp_days;
  unsigned int ntp_year, days_since_epoch, day_of_year; 

  String DateString = "", MonthString = "";

  leap_days=0; 
  leap_year_ind=0;
  
  ntp_second = epoch%60;
  epoch /= 60;
  ntp_minute = epoch%60;
  epoch /= 60;
  ntp_hour  = epoch%24;
  epoch /= 24;
    
  days_since_epoch = epoch;      //number of days since epoch
  ntp_week_day = week_days[days_since_epoch%7];  //Calculating WeekDay
  
  ntp_year = 1970+(days_since_epoch/365); // ball parking year, may not be accurate!

  int i;
  for (i=1972; i<ntp_year; i+=4)      // Calculating number of leap days since epoch/1970
    if(((i%4==0) && (i%100!=0)) || (i%400==0)) leap_days++;
        
  ntp_year = 1970+((days_since_epoch - leap_days)/365); // Calculating accurate current year by (days_since_epoch - extra leap days)
  day_of_year = ((days_since_epoch - leap_days)%365)+1;

  if(((ntp_year%4==0) && (ntp_year%100!=0)) || (ntp_year%400==0))  
  {
    month_days[1]=29;     //February = 29 days for leap years
    leap_year_ind = 1;    //if current year is leap, set indicator to 1 
    }
  else month_days[1]=28; //February = 28 days for non-leap years 

    temp_days=0;

  for (ntp_month=0 ; ntp_month <= 11 ; ntp_month++) //calculating current Month
    {
      if (day_of_year <= temp_days) break; 
      temp_days = temp_days + month_days[ntp_month];
    }
  
  temp_days = temp_days - month_days[ntp_month-1]; //calculating current Date
  ntp_date = day_of_year - temp_days;

  switch(ntp_month)
  {
    case 1: MonthString = "Jan";
    break;
    case 2: MonthString = "Fev";
    break;
    case 3: MonthString = "Mar";
    break;
    case 4: MonthString = "Abr";
    break;
    case 5: MonthString = "Mai";
    break;
    case 6: MonthString = "Jun";
    break;
    case 7: MonthString = "Jul";
    break;
    case 8: MonthString = "Ago";
    break;
    case 9: MonthString = "Set";
    break;
    case 10: MonthString = "Out";
    break;
    case 11: MonthString = "Nov";
    break;
    case 12: MonthString = "Dez";
    break;
  }

  DateString = (String) ntp_date + "/";
  DateString += (String) MonthString;
  
  return DateString;
}