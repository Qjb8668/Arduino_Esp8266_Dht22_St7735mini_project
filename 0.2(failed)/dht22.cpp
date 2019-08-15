#include "dht22.h"

DHT22::DHT22(byte _pin) {
	pin = _pin;
	humidity = 0;
	temperature = 0;
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}

DHT22::~DHT22() {
	digitalWrite(pin, LOW);
	pinMode(pin, INPUT); //disattach
}

byte DHT22::update() {
	//BUFFER TO RECEIVE
	/* unsigned char bit[5] = {0, 0, 0, 0, 0}
	unsigned char cnt = 7;
	unsigned char idx = 0;
	unsigned char sum; */
	unsigned char bits[5] = { 0, 0, 0, 0, 0 };
	unsigned char sum;
	unsigned int count = 10000;

	//STEP ONE: REQUEST
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	//SETP TWO: ACKOWLEDGE
	count = 10000;
	while (digitalRead(pin) == LOW)
		if (count-- == 0)
			return DHT_ERR_TIMEOUT;

	count = 10000;
	while (digitalRead(pin) == HIGH)
		if (count-- == 0)
			return DHT_ERR_TIMEOUT;

	Serial.println("Start Check!");

	//SETP THREE: READ
	for (int i = 0; i != 5; ++i) {
		for (int j = 7; j >= 0; j--) {
			count = 10000;
			while (digitalRead(pin) == LOW)
				if (count-- == 0)
					return DHT_ERR_TIMEOUT;

			unsigned long t_old = micros();
			count = 10000;
			while (digitalRead(pin) == HIGH)
				if (count-- == 0)
					return DHT_ERR_TIMEOUT;

			unsigned long t_new = micros();
			if ((t_new - t_old) > 40)
				bits[i] |= (1 << j);
		}
	}

	//SETP FOUR: CHECK
	sum = bits[0] + bits[1] + bits[2] + bits[3];
	if (bits[4] != sum)
		return DHT_ERR_CHECK;

	humidity = (float)((bits[0] << 8) + bits[1]) / 10;
	temperature = (float)((bits[2] << 8) + bits[3]) / 10;

	Serial.print("temperature:");
	Serial.println(temperature);
	Serial.print("humidity:");
	Serial.println(humidity);

	return DHT_OK;
}

float DHT22::getHumidity() {
	return humidity;
}

float DHT22::getTemperature() {
  return temperature;
}
