#include "time.h"

unsigned char packetBuffer[NTP_PACKET_SIZE];

void sendNTPPacket(IPAddress &address, WiFiUDP _udp) {
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	packetBuffer[0] = 0b11100011; //LI, Version, Mode
	packetBuffer[1] = 0; // Stratum, or type of clock
	packetBuffer[2] = 6; //Polling Interval
	packetBuffer[3] = 0xEC; //Peer Clock Precision
	//8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	_udp.beginPacket(address, 123);
	_udp.write(packetBuffer, NTP_PACKET_SIZE);
	_udp.endPacket();
}

time_t getNTPTime(WiFiUDP _udp) {
	IPAddress NTPServerIP;

	// Give up any data received before
	while (_udp.parsePacket() > 0);

	Serial.println("TRansmit NTP REquest");
	//Get ip from host
	WiFi.hostByName(NTPServerName, NTPServerIP);
	Serial.print(NTPServerName);
	Serial.print(":");
	Serial.println(NTPServerIP);
	sendNTPPacket(NTPServerIP, _udp);
	unsigned int t_begin = millis();
	while (millis() - t_begin < 1500) {
		int size = _udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			_udp.read(packetBuffer, NTP_PACKET_SIZE);
			unsigned long secsSince1900;
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			Serial.print("time:");
			Serial.println(secsSince1900);
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}

	Serial.println("No NTP Response!");
	return 0;
}

void getTime(int &_year, int &_month, int &_day, int &_weekday, int &_hour, 
	int &_minute, int &_second) {
	_year = year();
	_month = month();
	_day = day();
	_weekday = weekday();
	_hour = hour();
	_minute = minute();
	_second = second();
}
