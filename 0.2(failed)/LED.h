#pragma once

#ifndef _LED_H__
#define _LED_H__

#include "Arduino.h"

class LED {
private:
	byte pin;
	byte state; // 0 is off and >1 is on

public:
	LED(byte _pin = 0, byte _state = LOW);
	~LED();
	byte getPin();
	void on();
	void off();
	byte getState();
};

#endif // !_LED_H__

