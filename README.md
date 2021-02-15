# Clock

Clock using esp8266, NTP server and a 16x2 LCD display.

## Getting Started

This simple project is a clock. As main controller was used the esp8266, in which the current epoch time is obtained from a NTP server. The hour and date are shown in a 16x2 LCD display. The temperature is obtained by a **ds18b20** sensor wired at D4 pin. 

There are basically 5 screens:

### 1 - Boot screen: 
This screen appears when the system is powered up. The text is a "hello" mensage.

### 2 - Connections screen: 
It loads a bar graph while the Wifi is connecting

### 3 - IP screen: 
After connection, this screen shows the local IP. 

### 4 - Hour screen:
This is the main screen, showing the time. In this screen the is used big numbers, that completes the entire display.

### 5 - Date and temperature screen:
In this screen appears a temperature with only 2 digits, using the big number. Beside this, is shown also the current date and the day.

## Prerequisites
To use this project the following libraries are needed to install at arduino IDE:
```
NTPClient.h
BigFont02.h
DallasTemperature.h
```

