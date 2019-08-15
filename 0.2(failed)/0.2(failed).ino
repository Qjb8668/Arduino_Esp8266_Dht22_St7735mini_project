#include "Arduino.h"
#include "LED.h"
#include "LEDlib.h"
#include "dht22.h"
#include "time.h"
#include "lunar.h"
#include "wordlib.h"
#include "wifi.h"
#include "DisplayGroup.h"

//LED
#define LED_PIN 2
//DHT22
#define DHT_PIN 12
//ST7735
#define TFT_CS 15
#define TFT_RST 4 //-1 means to connect to Arduino RESET pin
#define TFT_DC 5
#define TFT_MOSI 13
#define TFT_SCLK 14
//Display
#define screenWeight 160
#define screenHeight 80
#define defaultColorBackground ST77XX_BLACK

DHT22 sensorTH(DHT22);
WiFiUDP udp;
LED led;
DHT22 sensor;
DisplayGroup group = DisplayGroup(screenWeight, screenHeight, defaultColorBackground, TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, 1);
Lunar lunarDates;

//global variable
int lyear = 1960, lmonth = 1, lday = 1, lweekday = 6, lhour = 0, lminute = 0, lsecond = 0;
int years = 1960, months = 1, days = 1, weekdays = 6, hours = 0, minutes = 0, seconds = 0;
float temperature, humidity;
int counts = 0;
unsigned long LastLoopTime = 0;
time_t PreviouDisplay = 0;
Lunar lunarBuffer[10];
int lunarBufferIndex = 0;

void displayInitialization() {
	/*
	first line: year, word_nian, months, word_yue, days, word_ri, two blanks, word_zhou,
	            weeks;
	second line: two blanks, first word of lyear, second word of lyear, word_nian,
	             lmonths, word_yue, first word of lday, second word of lday;
	third line: hours, ':', minutes, ':', seconds (mediate)
	forth line: 'temperature:', temperature;
	fifth line: 'humidity:', humidity;
	*/

	DisplayElement tmp(0, 0, 0.1, true, 1, ST7735_BLACK);
	for (int i = 0; i != 32; ++i)
		group.add(&tmp);
}

void insertWord(int index, int &x, int &y, int weight, int height, 
	char *content, unsigned int color) {
	group.list[index].setPos(x, y);
	group.list[index].changeContent(content, weight, height, color);
	x += 12;
}

void reflash() {
	getTime(years, months, days, weekdays, hours, minutes, seconds);
  unsigned char temp = false;
	for (; lunarBufferIndex != 0; lunarBufferIndex--) {
    if ((lunarBuffer[lunarBufferIndex].syear == years) && (lunarBuffer[lunarBufferIndex].smonth == months)
      && (lunarBuffer[lunarBufferIndex].sdate == days)) {
      memcpy(&lunarDates, &lunarBuffer[lunarBufferIndex], sizeof(Lunar));
      temp = true;
      break;
    }
  }
	if (!temp)
	  lunarDates = Lunar(years, months, days);
	sensor.update();

	// dates change
  int X = 0;
	if ((lyear == years) && (lmonth == months) && (lday == days) && (counts != 0)) {
		//print
		int firstLineY = 0; X = 0;
		group.list[0].setPos(X, firstLineY + 2);
		group.list[0].changeContent(years, 1, ST7735_YELLOW);
		X += 24;
		insertWord(1, X, firstLineY, WordWeight, WordHeight, Word_nian, ST7735_YELLOW);
		group.list[3].setPos(X, firstLineY + 2);
		group.list[3].changeContent(months, 1, ST7735_YELLOW);
		if (months >= 10)
			X += 12;
		else
			X += 6;
		insertWord(4, X, firstLineY, WordWeight, WordHeight, Word_yue, ST7735_YELLOW);
		group.list[5].setPos(X, firstLineY + 2);
		group.list[5].changeContent(days, 1, ST7735_YELLOW);
		if (months >= 10)
			X += 12;
		else
			X += 6;
		insertWord(6, X, firstLineY, WordWeight, WordHeight, Word_ri, ST7735_YELLOW);
		group.list[7].setPos(X, firstLineY + 2);
		group.list[7].changeContent('  ', 1, ST7735_YELLOW);
		X += 12;
		insertWord(8, X, firstLineY, WordWeight, WordHeight, Word_zhou, ST7735_YELLOW);
		insertWord(9, X, firstLineY, WordWeight, WordHeight,
			Word_xingqi[(weekdays + 5) % 7], ST7735_YELLOW);

		int secondLineY = 16; X = 0;
		int firstWordIndex = (lunarDates.year - 1901 + 7) % 10,
			secondWordIndex = (lunarDates.year - 1901 + 1) % 12;
		insertWord(10, X, secondLineY, WordWeight, WordHeight,
			Word_tian[firstWordIndex], ST7735_YELLOW);
		insertWord(11, X, secondLineY, WordWeight, WordHeight,
			Word_di[secondWordIndex], ST7735_YELLOW);
		insertWord(12, X, secondLineY, WordWeight, WordHeight, Word_nian, ST7735_YELLOW);
		if (lunarDates.isLeap) {
			insertWord(13, X, secondLineY, WordWeight, WordHeight, Word_run, ST7735_YELLOW);
			group.list[13].isVisual = true;
		}
		else
			group.list[13].isVisual = false;
		insertWord(14, X, secondLineY, WordWeight, WordHeight,
			Word_lunaryue[lunarDates.month - 1], ST7735_YELLOW);
		if (lunarDates.month == 11)
			insertWord(15, X, secondLineY, WordWeight, WordHeight,
				Word_xingqi[0], ST7735_YELLOW);
		insertWord(16, X, secondLineY, WordWeight, WordHeight, Word_yue, ST7735_YELLOW);
		if (lunarDates.date > 0 && lunarDates.date < 11) {
			insertWord(17, X, secondLineY, WordWeight, WordHeight, Word_chu, ST7735_YELLOW);
			if (lunarDates.date == 1)
				insertWord(18, X, secondLineY, WordWeight, WordHeight,
					Word_xingqi[1], ST7735_YELLOW);
			else
				insertWord(18, X, secondLineY, WordWeight, WordHeight,
					Word_lunaryue[lunarDates.date - 1], ST7735_YELLOW);
		}
		else if (lunarDates.date > 10 && lunarDates.date < 20) {
			insertWord(17, X, secondLineY, WordWeight, WordHeight, Word_lunaryue[9], ST7735_YELLOW);
			insertWord(18, X, secondLineY, WordWeight, WordHeight,
				Word_lunaryue[lunarDates.date - 11], ST7735_YELLOW);
		}
		else if (lunarDates.date == 20) {
			insertWord(17, X, secondLineY, WordWeight, WordHeight, Word_nian1, ST7735_YELLOW);
			insertWord(18, X, secondLineY, WordWeight, WordHeight, Word_lunaryue[9], ST7735_YELLOW);
		}
		else if (lunarDates.date > 20 && lunarDates.date < 30) {
			insertWord(17, X, secondLineY, WordWeight, WordHeight, Word_nian1, ST7735_YELLOW);
			insertWord(18, X, secondLineY, WordWeight, WordHeight,
				Word_lunaryue[lunarDates.date - 21], ST7735_YELLOW);
		}
		else if (lunarDates.date == 30) {
			insertWord(17, X, secondLineY, WordWeight, WordHeight, Word_lunaryue[2], ST7735_YELLOW);
			insertWord(18, X, secondLineY, WordWeight, WordHeight, Word_lunaryue[9], ST7735_YELLOW);
		}
	}
	
	int thirdLineY = 30; X = 32;
	if (lhour == hours)
		group.list[19].isFlash = false;
	else {
		group.list[19].isFlash = true;
		if ((hours >= 0) && (hours < 10)) {
			if (lhour >= 10)
				group.fillElement(19);
			X += 6;
			group.list[19].setPos(X, thirdLineY);
			group.list[19].changeContent(hours, 2, ST7735_YELLOW);
		}
		if ((hours >= 10) && (hours < 24)) {
			//if ((lhour / 10) == (hours / 10))
			group.list[19].setPos(X, thirdLineY);
			group.list[19].changeContent(hours, 2, ST7735_YELLOW);
		}
	}
	X = 56;
	group.list[20].setPos(X, thirdLineY);
	group.list[20].changeContent(':', 2, ST7735_YELLOW);
	X += 12;
	if (lminute == minutes)
		group.list[21].isFlash = false;
	else {
		group.list[21].isFlash = true;
		if ((minutes >= 0) && (minutes < 10)) {
			if (lminute >= 10)
				group.fillElement(20);
			X += 6;
			group.list[21].setPos(X, thirdLineY);
			group.list[21].changeContent(minutes, 2, ST7735_YELLOW);
		}
		if ((minutes >= 10) && (minutes < 60)) {
			//if ((lminute / 10) == (minute / 10))
			group.list[21].setPos(X, thirdLineY);
			group.list[21].changeContent(minutes, 2, ST7735_YELLOW);
		}
	}
	X = 80;
	group.list[22].setPos(X, thirdLineY);
	group.list[22].changeContent(':', 2, ST7735_YELLOW);
	X += 12;
	if (lsecond == seconds)
		group.list[23].isFlash = false;
	else {
		group.list[23].isFlash = true;
		if ((seconds >= 0) && (seconds < 10)) {
			if (lsecond >= 10)
				group.fillElement(23);
			X += 6;
			group.list[23].setPos(X, thirdLineY);
			group.list[23].changeContent(seconds, 2, ST7735_YELLOW);
		}
		if ((seconds >= 10) && (seconds < 60)) {
			//if ((lseconds / 10) == (seconds / 10))
			group.list[23].setPos(X, thirdLineY);
			group.list[23].changeContent(seconds, 2, ST7735_YELLOW);
		}
	}

	int forthLineY = 48; X = 0;
	group.list[24].setPos(X, forthLineY);
	group.list[24].changeContent('Temperature:', 1, ST7735_YELLOW);
	X += 72;
	group.list[25].setPos(X, forthLineY);
	group.list[25].changeContent(sensor.getTemperature(), 1, ST7735_YELLOW);
	
	int fifthLineY = 56; X = 0;
	group.list[26].setPos(X, fifthLineY);
	group.list[26].changeContent('Humidity:', 1, ST7735_YELLOW);
	X += 54;
	group.list[27].setPos(X, fifthLineY);
	group.list[27].changeContent(sensor.getHumidity(), 1, ST7735_YELLOW);

  group.flashAll();

	Serial.print(counts);
	Serial.print(":");
	Serial.println("Have been printed to TFT!");
	Serial.println("============end===============");

	lyear = years;
	lmonth = months;
	lday = days;
	lhour = hours;
	lminute = minutes;
	lsecond = seconds;
	counts += 1;

  if (lunarBufferIndex < 8)
    lunarBufferIndex++;
  memcpy(&lunarBuffer[lunarBufferIndex], &lunarDates, sizeof(Lunar));
}

time_t getNTPTime1() {
  return getNTPTime(udp);
}

void setup() {
	Serial.begin(9600);
	while (!Serial)
		continue;

	//LED setup
	Serial.println("LED setup started!");
	led = LED(LED_PIN);
	LED_star(led, 1, 500);
	Serial.println("LED setup finished!");
	delay(500);
	//DHT22 setup
	Serial.println("DHT22 setup started!");
	sensor = DHT22(DHT_PIN);
	LED_star(led, 1, 500);
	Serial.println("DHT22 setup finished!");
	delay(500);	
	//TFT setup
	Serial.println("TFT setup started!");
	group = DisplayGroup(screenWeight, screenHeight, defaultColorBackground,
		TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, 1);
	LED_star(led, 1, 500);
	Serial.println("TFT setup finished!");
	delay(500);
	//ESP8266 and NTP setup
	Serial.println("ESP8266 setup started!");
	wifiSetu(udp);
	Serial.println("waiting for sync");
	setSyncProvider(getNTPTime1);
	setSyncInterval(300);
	delay(500);
	Serial.println("ESP8266 setup finished");
	//display initialization
	displayInitialization();
}

void loop() {
	// if interval is less than 1000ms, exit
	if ((millis() - LastLoopTime) < 900) {
		delay(50);
		return;
	}
	unsigned long t_old = millis();

	unsigned char isErr = 0;

	int DHT_state = sensor.update();
	if (DHT_state != DHT_OK)
		isErr = DHT_state;

	if (timeStatus() != timeNotSet) {
		if (now() != PreviouDisplay)
			PreviouDisplay = now();
		else
			isErr = 3;
	}

	if (isErr == 0)
		reflash();
	else {
		LED_star(led, 2, 100);
		printConsole("ERR code:", isErr);
	}

	LastLoopTime = millis();
	unsigned long t_new = millis();
	printConsole("time:", t_new - t_old);
}
