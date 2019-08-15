#include "LEDlib.h"

void LED_star(LED _led, int round, int DelayLength) {
	//if round is negtive means LED to be shankle continous
	//DelayLength is not allowed to shorter than 250ms or will be change to default Delaylength
	if (DelayLength < 250)
		DelayLength = 250;

	//Do it
	for (int i = 0; i != round; ++i) {
		_led.on();
		delay(DelayLength);
		_led.off();
		delay(DelayLength);
	}
}
