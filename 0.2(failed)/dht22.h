#ifndef _DHT22_H
#define _DHT22_H

#include <Arduino.h>

#define DHT_OK 0
#define DHT_ERR_CHECK 2
#define DHT_ERR_TIMEOUT 1

class DHT22 {
private:
	byte pin;
	float humidity, temperature;

public:
	DHT22(byte _pin = 0);
	virtual ~DHT22();
	byte update();
	float getHumidity();
	float getTemperature();
};
#endif // !_DHT22_H
