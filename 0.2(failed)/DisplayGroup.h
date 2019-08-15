#ifndef _DISPLAY_GROUP_H
#define _DISPLAY_GROUP_H

#include "DisplayElement.h"
#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "SPI.h"
#include <vector>
#include <iterator>
#include "debug.h"
using namespace std;

class DisplayGroup {
private:
	unsigned char num;
	int weight, height;
	unsigned int colorBackground;
	// unsigned char **colorPixels;
	Adafruit_ST7735 tft = Adafruit_ST7735(15, 5, 13, 14, 4);

	void flashElement(DisplayElement *thisElement);

public:
  vector <DisplayElement > list;
	DisplayGroup(int _weight, int _height, unsigned char _colorBackground,
		int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst, int8_t rotation = 1);
	~DisplayGroup();
	void add(DisplayElement *newElement);
	void flashAll();
	void fillElement(int index);
};
#endif // !_DISPLAY_GROUP_H
