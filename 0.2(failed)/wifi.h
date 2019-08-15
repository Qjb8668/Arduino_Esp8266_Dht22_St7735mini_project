#ifndef _WIFI_H
#define _WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "debug.h"

//WiFi SETTING
#define WiFiCount 3
const char ssid[WiFiCount][32] = { {"DESKTOP-A7IP9AM 5614"}, {"mi"}, {"USER_059741"} };
const char password[WiFiCount][32] = { {"82268129"}, {"82268129"}, {"qq82268129"} };
//WiFiUDP SETTING
#define myLocalPort 8888
#define scanLimit 7

void wifiSetu(WiFiUDP &_udp);
#endif // !_WIFI_H
