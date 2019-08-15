#include "wifi.h"

void wifiSetu(WiFiUDP &_udp) {
	Serial.print("Connecting WiFi...");
	WiFi.mode(WIFI_STA);
	unsigned long t_old = millis();
	for (int i = 0; true; ) {
		WiFi.begin(ssid[i], password[i]);
		for (int j = 0; j != 10; ++j) {
			if (WiFi.status() != WL_CONNECTED) {
				delay(500);
				Serial.print(".");
			}
			else
				break;
		}
		if ((WiFi.status() != WL_CONNECTED) && ((millis() - t_old) <= 10000)) {
			WiFi.disconnect();
			i = (i + 1) % WiFiCount;
		}
		else
			break;
	}
	Serial.println("");
	Serial.println("WiFi connected!");
	printConsole("WiFi ssid: ", WiFi.SSID().c_str());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
	Serial.print("Starting UDP: ");
	_udp.begin(myLocalPort);
	// Serial.println(_udp.localPort());
}
