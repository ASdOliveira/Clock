#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SENSOR_PIN D4
#define LED_PIN D0

#define WIFI_HOTSPOT_NAME "WIFI_CLOCK"

#define TIME_TO_WAIT_WIFI_CONNECTION 3000
#define TIME_TO_SYNC_NTP 30000    // Time in microseconds to update date and time

#define DATETIME_PERIOD 3500       // number of milliseconds showing the clock
#define TEMPERATURE_PERIOD 2500    // number of milliseconds showing the Temp
#define TOTAL_PERIOD (DATETIME_PERIOD + TEMPERATURE_PERIOD)

#define UPDATE_SCREEN_TIME  0.8    //Value in seconds

#endif