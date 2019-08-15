#include "DisplayElement.h"

unsigned char DisplayElement::getNumberLen(int num) {
	unsigned char len = 1;
	for (; num >= 10; num /= 10)
		len++;
	return len;
}

/*
DisplayElement::DisplayElement(int _posX, int _posY, int *_number,
	unsigned char _isVisual, unsigned char _size, 
	unsigned int _color) {
	posX = _posX;
	posY = _posY;
	type = 1;
	unsigned charlen = getNumberLen(_number);
	weight = len * charWeight * _size;
	height = len * charHeight * _size;
	size = _size;
	number = _number;
	numberFloat = 0;
	strContent = NULL;
	content = NULL;
	color = _color;
	isVisual = _isVisual;
	isFlash = true;
} // number initialization
*/

DisplayElement::DisplayElement(int _posX, int _posY, char _content[],
	unsigned char _isVisual, unsigned char _size,
	unsigned int _color) {
	posX = _posX;
	posY = _posY;
	type = 2;
	unsigned len = strlen(_content);
	weight = len * charWeight * _size;
	height = len * charHeight * _size;
	size = _size;
	number = 0;
	numberFloat = 0;
	content = _content;
	color = _color;
	isVisual = _isVisual;
	isFlash = true;
} // string initialization

DisplayElement::DisplayElement(int _posX, int _posY, int _weight, int _height, 
	char *_content, unsigned _isVisual,
	unsigned int _color) {
	posX = _posX;
	posY = _posY;
	weight = _weight;
	height = _height;
	type = 3;
	size = 1;
	number = 0;
	numberFloat = 0;
	content = _content;
	color = _color;
	isVisual = _isVisual;
	isFlash = true;
} // word initialization

DisplayElement::DisplayElement(int _posX, int _posY, float _numberFloat,
	unsigned char _isVisual, unsigned char _size,
	unsigned int _color) {
	posX = _posX;
	posY = _posY;
	type = 4;
	weight = 5 * charWeight * _size;
	height = 5 * charHeight * _size;
	size = _size;
	numberFloat = _numberFloat;
	number = 0;
	content = NULL;
	color = _color;
	isVisual = _isVisual;
	isFlash = true;
} // number of float initialization

DisplayElement::~DisplayElement() {
	//
}

int DisplayElement::getPosX() {
	return posX;
}

int DisplayElement::getPosY() {
	return posY;
}

int DisplayElement::getWeight() {
	return weight;
}

int DisplayElement::getHeight() {
	return height;
}

unsigned int DisplayElement::getColor() {
	return color;
}

unsigned char DisplayElement::getSize() {
	return size;
}

unsigned char DisplayElement::getIsDisplay() {
	return isVisual;
}

unsigned char DisplayElement::getIsFlash() {
	return isFlash;
}

void DisplayElement::setPos(int x, int y) {
	posX = x;
	posY = y;
}

/*
void DisplayElement::changeContent(int _number, unsigned char size,
	unsigned char _color) {
	type = 1;
	unsigned charlen = getNumberLen(_number);
	weight = len * charWeight * size;
	height = len * charHeight * size;
	number = _number;
	numberFloat = 0;
	*content = NULL;
	color = _color;
} // change to number
*/

void DisplayElement::changeContent(char _content[], unsigned char size,
	unsigned int _color) {
	type = 2;
	unsigned len = strlen(_content);
	weight = len * charWeight * size;
	height = len * charHeight * size;
	number = 0;
	numberFloat = 0;
	content = _content;
	color = _color;
} // change to string

void DisplayElement::changeContent(char *_content, int _weight, int _height,
	unsigned int _color) {
	type = 3;
	weight = _weight;
	height = _height;
	size = 0;
	number = 0;
	numberFloat = 0;
	content = _content;
	color = _color;
} // change to word

void DisplayElement::changeContent(float _numberFloat, unsigned char size,
	unsigned int _color) {
	type = 1;
	weight = 5 * charWeight * size;
	height = 5 * charHeight * size;
	numberFloat = _numberFloat;
	*content = NULL;
	color = _color;
} // change to float number
