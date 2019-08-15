#ifndef _DISPLAY_ELEMENT_H
#define _DISPLAY_ELEMENT_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "SPI.h"
#include "string.h"

#define charWeight 6
#define charHeight 8

class DisplayElement {
private:
	int posX, posY;
	int weight, height;
	unsigned char size;
	unsigned int color;
	
	unsigned char getNumberLen(int num);

public:
  unsigned char isVisual;
	unsigned char type; // 1 is num, 2 is a string, 3 is a array, 4 is float;
	int number;
	float numberFloat;
	char *content;
	unsigned char isFlash;

	/*
	DisplayElement(int _posX, int _posY, int *_number,
		unsigned char _isDisplay = true, unsigned char size = 1,
		unsigned int _color = ST7735_BLACK);
    */
	DisplayElement(int _posX, int _posY, char _content[],
		unsigned char _isVisual = true, unsigned char size = 1,
		unsigned int _color = ST7735_BLACK);
	DisplayElement(int _posX, int _posY, int _weight, int _height,
		char *_content, unsigned _isVisual = true,
		unsigned int _color = ST7735_BLACK);
	DisplayElement(int _posX, int _posY, float _numberFloat,
		unsigned char _isVisual = true, unsigned char size = 1,
		unsigned int _color = ST7735_BLACK);
	virtual ~DisplayElement();
	int getPosX();
	int getPosY();
	int getWeight();
	int getHeight();
	unsigned int getColor();
	unsigned char getSize();
	unsigned char getIsDisplay();
	unsigned char getIsFlash();
	void setPos(int x, int y);
	/*
   void changeContent(int _number, unsigned char size = 1,
    unsigned int _color = ST7735_BLACK);
	*/
	void changeContent(char _content[], unsigned char size = 1,
		unsigned int _color = ST7735_BLACK);
	void changeContent(char *_content, int _weight, int _height,
		unsigned int _color = ST7735_BLACK);
	void changeContent(float _numberFloat, unsigned char size = 1,
		unsigned int _color = ST7735_BLACK);
};

#endif // !_DISPLAY_ELEMENT_H
