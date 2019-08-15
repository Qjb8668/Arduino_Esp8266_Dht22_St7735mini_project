#include "LED.h"

LED::LED(byte _pin, byte _state) {
	pin = _pin;
	state = _state;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, state);
}

LED::~LED() {
	digitalWrite(pin, LOW);
	pinMode(pin, INPUT); // disattach
}

byte LED::getPin() {
	return pin;
}

byte LED::getState() {
	if (state)
		return HIGH;
	else
		return LOW;
}

void LED::on() {
	state = HIGH;
	digitalWrite(pin, HIGH);
}

void LED::off() {
	state = LOW;
	digitalWrite(pin, LOW);
}
