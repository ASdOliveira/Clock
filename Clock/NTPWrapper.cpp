#include <time.h>
#include "NTPWrapper.h"
#include "LED.h"
#include "Defines.h"

const long utcOffsetInSeconds = -(3600*3);
const char daysOfTheWeek[7][8] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, TIME_TO_SYNC_NTP);
LED led(LED_PIN);

NTPWrapper::NTPWrapper()
{
  timeClient.begin(); // Connect to NTP server
}

void NTPWrapper::Update()
{
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
}

void NTPWrapper::ForceUpdate()
{
  while(!timeClient.update())
  {
    timeClient.forceUpdate();
    led.Blink();
    delay(1500);
  }
}

int NTPWrapper::GetHours()
{
  return timeClient.getHours();
}

int NTPWrapper::GetMinutes()
{
  return timeClient.getMinutes();
}

String NTPWrapper::GetDay()
{
  return daysOfTheWeek[timeClient.getDay()];
}

String NTPWrapper::GetDate()
{
  return getDateFromEpoch(timeClient.getEpochTime());
}

String NTPWrapper::getDateFromEpoch(unsigned long epoch)
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