# Clock

Clock using esp8266, NTP server and a 16x2 LCD display.

## Getting Started

This simple project is a clock. As main controller was used the esp8266, in which the current epoch time is obtained from a NTP server. The hour and date are shown in a 16x2 LCD display. The temperature is obtained by a **ds18b20** sensor wired at D4 pin. 

### Task list:

- [X] Organize the code into files
- [X] Add wifiManager 
- [ ] ~Separate date and temperature on different screens~
- [ ] ~Add Icons to date and temperature screens~
- [ ] Add animation in hour screen~
- [X] Improve readme file