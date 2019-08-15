#include "DisplayGroup.h"

DisplayGroup::DisplayGroup(int _weight, int _height, unsigned char _colorBackground,
	int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst, int8_t rotation) {
	num = 0;
	list.clear();
	weight = _weight;
	height = _height;
	colorBackground = _colorBackground;
	/*
	**colorPixels  = new unsigned char [height][weight];
	for (int i=0; i != height; ++i)
		for (int j=0; j != weight; ++j)
			colorPixels[i][j]  = colorBackground;
	*/
	
	// tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);
	tft.initR(INITR_MINI160x80);
	tft.setRotation(rotation);
	printConsole("Width:", tft.width());
	printConsole("Height:", tft.height());
	unsigned long t_old = micros();
	tft.fillScreen(ST77XX_BLUE);
	unsigned long t_new = micros();
	printConsole("TFT fillScreen:", t_new - t_old);
}

DisplayGroup::~DisplayGroup() {
	// 
}

void DisplayGroup::flashElement(DisplayElement *thisElement) {
	int x = thisElement->getPosX(), y = thisElement->getPosY();
	int elementWeight = thisElement->getWeight(), elementHeight = thisElement->getHeight();
	
	tft.setCursor(x, y);
	tft.setTextSize(thisElement->getSize());
	tft.fillRect(x, y, elementWeight, elementHeight, colorBackground);

	if ((thisElement->type) == 1)
		tft.print(thisElement->number);
	if ((thisElement->type) == 2)
		tft.print(*(thisElement->content));
	if ((thisElement->type) == 3)
		tft.drawBitmap(x, y,(uint8_t*) thisElement->content, elementWeight, elementHeight, thisElement->getColor());

	thisElement->isFlash = true;
}

void DisplayGroup::add(DisplayElement *newElement) {
	num++;
	list.push_back(*newElement);
}

void DisplayGroup::flashAll() {
	for (int i = 0; i != num; ++i)
		if (list[i].isFlash)
			flashElement(&list[i]);
}

void DisplayGroup::fillElement(int index) {
	int x = list[index].getPosX(), y = list[index].getPosY();
	int elementWeight = list[index].getWeight(), elementHeight = list[index].getHeight();
	tft.fillRect(x, y, elementWeight, elementHeight, colorBackground);
}
