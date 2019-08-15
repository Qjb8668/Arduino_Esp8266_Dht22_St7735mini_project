#ifndef _TIME_H
#define _TIME_H

#include <Arduino.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

//NTP SETTING
#define NTPServerName "ntp.sjtu.edu.cn"
#define timeZone 8
#define NTP_PACKET_SIZE 48

extern unsigned char packetBuffer[NTP_PACKET_SIZE];

void sendNTPPacket(IPAddress &address, WiFiUDP _udp);
time_t getNTPTime(WiFiUDP _udp);
void getTime(int &_year, int &_month, int &_day, int &_weekday, int &_hour, 
	int &_minute, int &_second);

#endif // !_TIME_H
