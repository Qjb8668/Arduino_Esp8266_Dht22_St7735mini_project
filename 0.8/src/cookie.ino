#include <inttypes.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <math.h>
#include <PubSubClient.h>
#include "DHT.h"

//CONST DEFINE
#define WordWeight 12
#define WordHeight 12
const unsigned char Word_nian[] = {
	0x00,0x00,0x08,0x80,0x0F,0x00,0x12,0x00,0x0F,0x00,0x12,0x00,0x1F,0xC0,0x22,0x00,
	0x02,0x00,0x02,0x00,0x02,0x00,0x00,0x00/*"nian"*/
};
const unsigned char Word_yue[] = {
	0x00,0x00,0x03,0x80,0x0C,0x80,0x09,0x00,0x0D,0x00,0x09,0x00,0x0D,0x00,0x10,0x80,
	0x10,0x80,0x23,0x80,0x41,0x00,0x00,0x00/*"yue"*/
};
const unsigned char Word_ri[] = {
	0x00,0x00,0x00,0x00,0x1F,0x80,0x10,0x80,0x11,0x00,0x0D,0x00,0x11,0x00,0x11,0x00,
	0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00/*"ri"*/
};
const unsigned char Word_xing[] = {
	0x00,0x00,0x03,0x80,0x1E,0x80,0x09,0x00,0x06,0x00,0x12,0x00,0x1F,0x00,0x25,0x00,
	0x06,0x00,0x05,0xE0,0x7A,0x00,0x00,0x00/*"xing"*/
};
const unsigned char Word_qi[]{
	0x00,0x00,0x14,0x00,0x17,0xE0,0x1D,0x20,0x15,0x20,0x1D,0xE0,0x17,0x20,0x69,0xE0,
	0x15,0x20,0x62,0x20,0x04,0x40,0x00,0x00,/*"qi"*/
};
const unsigned char Word_zhou[] = {
	0x00,0x00,0x1F,0xC0,0x12,0x40,0x1E,0x40,0x14,0x40,0x1B,0x40,0x1F,0x40,0x29,0x40,
	0x26,0x40,0x40,0x40,0x00,0xC0,0x00,0x00/*"zhou"*/
};
const unsigned char Word_chu[] = {
	0x00,0x00,0x0C,0x00,0x00,0x00,0x0C,0x60,0x78,0xA0,0x0D,0x20,0x3D,0x20,0x4A,0x40,
	0x0C,0xC0,0x00,0x00,0x00,0x00,0x00,0x00/*"chu"*/
};
const unsigned char Word_nian1[] = {
	0x00,0x00,0x01,0x00,0x10,0x80,0x09,0x00,0x09,0x60,0x7F,0x80,0x09,0x00,0x09,0x00,
	0x09,0x00,0x0E,0x00,0x00,0x00,0x00,0x00/*"nian"*/
};
const unsigned char Word_run[] = {
	0x00,0x00,0x10,0xC0,0x03,0x40,0x20,0x40,0x26,0x40,0x26,0x40,0x24,0x40,0x3F,0xC0,
	0x20,0x40,0x20,0xC0,0x00,0x40,0x00,0x00/*"run"*/
};
const unsigned char Word_tian[10][24] = {
{
	0x00,0x00,0x01,0xC0,0x3E,0x40,0x23,0x40,0x24,0x80,0x23,0x80,0x1C,0x00,0x04,0x00,
	0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00/*"jia"*/
},
{
	0x00,0x00,0x00,0x00,0x1E,0x00,0x08,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x20,0x40,
	0x20,0x60,0x1F,0x80,0x00,0x00,0x00,0x00/*"yi"*/
},
{
	0x00,0x00,0x00,0x40,0x7F,0x80,0x04,0x00,0x05,0xC0,0x3E,0x40,0x26,0x40,0x29,0x40,
	0x30,0x40,0x21,0xC0,0x00,0x00,0x00,0x00/*"bing"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0xC0,0x22,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
	0x02,0x00,0x06,0x00,0x00,0x00,0x00,0x00/*"ding"*/
},
{
	0x04,0x00,0x04,0xC0,0x04,0x00,0x06,0x00,0x1C,0x00,0x12,0x80,0x12,0x80,0x21,0x00,
	0x23,0x00,0x40,0xA0,0x00,0x60,0x00,0x00/*"wu"*/
},
{
	0x00,0x00,0x02,0x00,0x1E,0x00,0x02,0x00,0x3C,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
	0x20,0x40,0x1F,0x80,0x00,0x00,0x00,0x00/*"ji"*/
},
{
	0x04,0x00,0x02,0x00,0x17,0x00,0x1A,0x00,0x17,0x80,0x13,0xE0,0x2D,0x00,0x2A,0x00,
	0x25,0x00,0x48,0x80,0x00,0x60,0x00,0x00/*"geng"*/
},
{
	0x00,0x00,0x06,0x80,0x1B,0x00,0x0A,0x00,0x04,0xE0,0x7F,0x00,0x07,0x80,0x1C,0x00,
	0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00/*"xin"*/
},
{
	0x00,0x00,0x01,0x00,0x02,0x00,0x06,0x00,0x04,0x00,0x03,0xE0,0x7C,0x00,0x04,0x00,
	0x04,0x00,0x1B,0x80,0x00,0x00,0x00,0x00/*"ren"*/
},
{
	0x00,0x00,0x0B,0x80,0x0A,0x80,0x11,0x00,0x31,0x00,0x2C,0xC0,0x24,0x70,0x7F,0x80,
	0x0A,0x00,0x09,0x80,0x30,0x00,0x00,0x00/*"kui"*/
}
};
const unsigned char Word_di[12][24] = {
{
	0x00,0x00,0x03,0x00,0x0D,0x00,0x02,0x00,0x02,0xE0,0x7F,0x00,0x02,0x00,0x02,0x00,
	0x02,0x00,0x02,0x00,0x04,0x00,0x00,0x00/*"zi"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x80,0x04,0x80,0x08,0x80,0x07,0x00,0x09,0x00,0x09,0x00,
	0x11,0xE0,0x7E,0x00,0x00,0x00,0x00,0x00/*"chou"*/
},
{
	0x04,0x00,0x04,0x40,0x3B,0xE0,0x06,0x00,0x04,0x00,0x1F,0x80,0x17,0x00,0x1F,0x00,
	0x10,0x00,0x11,0x80,0x20,0x80,0x00,0x00/*"ming"*/
},
{
	0x00,0x00,0x08,0x00,0x33,0xE0,0x2A,0x40,0x2A,0x40,0x2A,0xC0,0x3A,0x00,0x0A,0x00,
	0x12,0x00,0x22,0x00,0x00,0x00,0x00,0x00/*"mao"*/
},
{
	0x00,0x00,0x0F,0x00,0x10,0x00,0x16,0x00,0x11,0x80,0x1E,0x40,0x1A,0x80,0x29,0x00,
	0x48,0x80,0x8C,0x70,0x08,0x00,0x00,0x00/*"chen"*/
},
{
	0x00,0x00,0x00,0x00,0x3E,0x00,0x24,0x00,0x24,0x00,0x38,0x00,0x20,0x00,0x20,0x40,
	0x20,0x60,0x1F,0x80,0x00,0x00,0x00,0x00/*"ji"*/
},
{
	0x00,0x00,0x08,0x00,0x09,0x80,0x16,0x00,0x02,0x00,0x0F,0xE0,0x32,0x00,0x02,0x00,
	0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00/*"wu"*/
},
{
	0x04,0x00,0x04,0x00,0x07,0x00,0x04,0x00,0x07,0x00,0x3C,0x00,0x0E,0x00,0x15,0x00,
	0x24,0xC0,0x04,0x20,0x04,0x00,0x00,0x00/*"wei"*/
},
{
	0x04,0x00,0x04,0x00,0x04,0x00,0x3F,0xC0,0x25,0x40,0x26,0x80,0x17,0x80,0x1C,0x00,
	0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00/*"shen"*/
},
{
	0x00,0x00,0x03,0xE0,0x3E,0x00,0x0A,0x00,0x1F,0x80,0x1A,0x80,0x19,0x80,0x16,0x80,
	0x28,0x80,0x1F,0x80,0x00,0x80,0x00,0x00/*"xi"*/
},
{
	0x04,0x00,0x04,0xC0,0x04,0x00,0x03,0x00,0x3E,0x00,0x22,0x80,0x27,0x00,0x39,0x00,
	0x22,0xA0,0x40,0x60,0x00,0x20,0x00,0x00/*"xu"*/
},
{
	0x00,0x00,0x02,0x00,0x00,0x60,0x7F,0x80,0x05,0x00,0x0A,0x00,0x1C,0x80,0x05,0x00,
	0x1B,0x00,0x24,0x80,0x00,0x00,0x00,0x00/*"hai"*/
}
};
const unsigned char Word_lunaryue[12][24] = {
{
	0x00,0x00,0x00,0x00,0x07,0x00,0x02,0x00,0x02,0x00,0x13,0x80,0x12,0x00,0x12,0x00,
	0x13,0xE0,0x6C,0x00,0x00,0x00,0x00,0x00/*"zheng"*/
},
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,
	0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"er"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,
	0x07,0xE0,0x78,0x00,0x00,0x00,0x00,0x00/*"san"*/
},
{
	0x00,0x00,0x00,0x00,0x01,0xC0,0x7E,0x20,0x2A,0x40,0x32,0xC0,0x31,0x40,0x23,0x40,
	0x1C,0xC0,0x00,0x00,0x00,0x00,0x00,0x00/*"si"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x00,0x04,0x00,0x04,0x00,0x07,0x80,0x09,0x00,0x09,0x00,
	0x09,0x60,0x76,0x80,0x00,0x00,0x00,0x00/*"wu"*/
},
{
	0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0xE0,0x3C,0x00,0x08,0x00,0x09,0x80,
	0x10,0x40,0x20,0x00,0x00,0x00,0x00,0x00/*"liu"*/
},
{
	0x00,0x00,0x08,0x00,0x08,0x00,0x08,0x40,0x09,0x80,0x0E,0x00,0x78,0x00,0x08,0x00,
	0x04,0x60,0x03,0x80,0x00,0x00,0x00,0x00/*"qi"*/
},
{
	0x00,0x00,0x00,0x00,0x04,0x00,0x04,0x00,0x02,0x00,0x09,0x00,0x11,0x00,0x20,0xC0,
	0x40,0x60,0x00,0x00,0x00,0x00,0x00,0x00/*"ba"*/
},
{
	0x00,0x00,0x04,0x00,0x08,0x00,0x0B,0x00,0x3D,0x00,0x0A,0x00,0x12,0x00,0x12,0x00,
	0x22,0x20,0x41,0xE0,0x00,0x00,0x00,0x00/*"jiu"*/
},
{
	0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x1F,0xC0,0x24,0x00,0x04,0x00,0x04,0x00,
	0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00/*"shi"*/
},
{
	0x00,0x00,0x02,0x00,0x02,0x00,0x04,0x00,0x1F,0xC0,0x24,0x00,0x04,0x00,0x04,0x00,
	0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00/*"shi"*/
},
{
	0x00,0x00,0x00,0x80,0x3A,0xC0,0x37,0x80,0x32,0xE0,0x31,0x00,0x32,0xC0,0x55,0x40,
	0x56,0x40,0x11,0xC0,0x00,0x00,0x00,0x00/*"la"*/
}
};
const unsigned char Word_xingqi[7][24] = {
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"yi"*/
},
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,
	0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"er"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,
	0x07,0xE0,0x78,0x00,0x00,0x00,0x00,0x00/*"san"*/
},
{
	0x00,0x00,0x00,0x00,0x01,0xC0,0x7E,0x20,0x2A,0x40,0x32,0xC0,0x31,0x40,0x23,0x40,
	0x1C,0xC0,0x00,0x00,0x00,0x00,0x00,0x00/*"si"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x00,0x04,0x00,0x04,0x00,0x07,0x80,0x09,0x00,0x09,0x00,
	0x09,0x60,0x76,0x80,0x00,0x00,0x00,0x00/*"wu"*/
},
{
	0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x03,0xE0,0x3C,0x00,0x08,0x00,0x09,0x80,
	0x10,0x40,0x20,0x00,0x00,0x00,0x00,0x00/*"liu"*/
},
{
	0x00,0x00,0x00,0x00,0x1F,0x80,0x10,0x80,0x11,0x00,0x0D,0x00,0x11,0x00,0x11,0x00,
	0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00/*"ri"*/
}
};

//Lunar dates
/*
  bit23 bit22 bit21 bit20:means the leap month this year 
						  while 0 means there is no leap month this year
  bit19 bit18 bit17 bit16 bit15 bit14 bit13 bit12 bit11 bit10 bit9 bit8 bit7
	1     2     3     4     5     6     7     8     9     10    11   12   13
  the amount of this month while 1 means 30 days and 0 means 29 days
  bit6 bit5                         bit4 bit3 bit2 bit1 bit0
  solor month of Spring Fesital   solor dates of Spring Festival
*/
unsigned int lunar200y[199] = {
	0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,/*1901-1910*/
	0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,/*1911-1920*/
	0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,/*1921-1930*/
	0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,/*1931-1940*/
	0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,/*1941-1950*/
	0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,/*1951-1960*/
	0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,/*1961-1970*/
	0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,/*1971-1980*/
	0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,/*1981-1990*/
	0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,/*1991-2000*/
	0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,/*2001-2010*/
	0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,/*2011-2020*/
	0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,/*2021-2030*/
	0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,/*2031-2040*/
	0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,/*2041-2050*/
	0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,/*2051-2060*/
	0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,/*2061-2070*/
	0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,/*2071-2080*/
	0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,/*2081-2090*/
	0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5          /*2091-2099*/
};
//class of lunar
class Lunar {
	public:
		int syear, smonth, sdate;
		int year, month, date;
		bool isLeap;
		Lunar();
		Lunar(int _year, int _month, int _day);
	private:
		int getLunarMonthDays(int _year, int _monthIndex);
};
int LunarBufferIndex = 0;
Lunar LunarBuffer[10];
//bit 1 means Line 1, bit 2 means Line two, bit 3 and bit 4 means hours first and second number,
//bit 5 and 6 means minutes fir and second number, bit 7 and 8 means seconds first and second number
unsigned char isDisplay = 0xff;


//LED
#define LED_PIN 2

//DHT22
//PIN NUM
#define DHT_PIN 12
#define DHT_TYPE DHT22
//STATE NUM
#define DHT_OK 0
#define DHT_ERR_CHECK 2
#define DHT_ERR_TIMEOUT 1
//global variable
DHT dht22(DHT_PIN, DHT_TYPE);
float humidity, temperature;

//ST7735
//PIN NUM
#define TFT_CS 15
#define TFT_RST 4 //-1 means to connect to Arduino RESET pin
#define TFT_DC 5
#define TFT_MOSI 13
#define TFT_SCLK 14
//TFT OBJECT
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//ESP8266
//WiFi SETTING
#define WiFiCount 3
#define NTPServerName "cn.ntp.org.cn"
#define timeZone 8
const char ssid[WiFiCount][32] = { {"DESKTOP-A7IP9AM 5614"}, {"mi"}, {"USER_059741"} };
const char password[WiFiCount][32] = { {"82268129"}, {"82268129"}, {"qq82268129"} };
//WiFiUDP SETTING
#define myLocalPort 8888
#define scanLimit 7
//NTP SETTING
#define NTP_PACKET_SIZE 48
//global variable
WiFiUDP Udp;
unsigned char packetBuffer[NTP_PACKET_SIZE];
//Upload Module
#define TOKEN "ESP8266_DEVICE_TOKEN"
WiFiClient espClient;
PubSubClient client(espClient);
char thingsboardServer[] = "203.195.201.32";
time_t PreviouDisplay = 0;

//global variable
int count = 0;
unsigned long LastLoopTime = 0;

void LED_star(int round, int DelayLength) {
	//if round is negtive means LED to be shankle continous
	//DelayLength is not allowed to shorter than 250ms or will be change to default Delaylength
	if (DelayLength < 250)
		DelayLength = 250;

	//Do it
	pinMode(LED_PIN, OUTPUT);
	for (int i = 0; i != round; ++i) {
		digitalWrite(LED_PIN, HIGH);
		delay(DelayLength);
		digitalWrite(LED_PIN, LOW);
		delay(DelayLength);
	}
	
}

//int DHT_read() {
//	//BUFFER TO RECEIVE
//	/* unsigned char bit[5] = {0, 0, 0, 0, 0}
//	unsigned char cnt = 7;
//	unsigned char idx = 0;
//	unsigned char sum; */
//	unsigned char bits[5] = {0, 0, 0, 0, 0};
//	unsigned char sum;
//	unsigned int count = 10000;
//
//	//STEP ONE: REQUEST
//	pinMode(DHT_PIN, OUTPUT);
//	digitalWrite(DHT_PIN, LOW);
//	delay(18);
//	digitalWrite(DHT_PIN, HIGH);
//	delayMicroseconds(40);
//	pinMode(DHT_PIN, INPUT);
//
//	//SETP TWO: ACKOWLEDGE
//	count = 10000;
//	while (digitalRead(DHT_PIN) == LOW)
//		if (count-- == 0)
//			return DHT_ERR_TIMEOUT;
//
//	count = 10000;
//	while (digitalRead(DHT_PIN) == HIGH)
//		if (count-- == 0)
//			return DHT_ERR_TIMEOUT;
//
//	Serial.println("Start Check!");
//
//	//SETP THREE: READ
//	for (int i = 0; i != 5; ++i) {
//		for (int j = 7; j >= 0; j--) {
//			count = 10000;
//			while (digitalRead(DHT_PIN) == LOW)
//				if (count-- == 0)
//					return DHT_ERR_TIMEOUT;
//
//			unsigned long t_old = micros();
//			count = 10000;
//			while (digitalRead(DHT_PIN) == HIGH)
//				if (count-- == 0)
//					return DHT_ERR_TIMEOUT;
//
//			unsigned long t_new = micros();
//			if ((t_new - t_old) > 40)
//				bits[i] |= (1 << j);
//		}
//	}
//
//	//SETP FOUR: CHECK
//	sum = bits[0] + bits[1] + bits[2] + bits[3];
//	if (bits[4] != sum)
//		return DHT_ERR_CHECK;
// 
//	humidity = (float)((bits[0] << 8) + bits[1]) / 10;
//	temperature = (float)((bits[2] << 8) + bits[3]) / 10;
//
//	Serial.print("temperature:");
//	Serial.println(temperature);
//	Serial.print("humidity:");
//	Serial.println(humidity);
//	
//	return DHT_OK;
//}

int DHT_read() {
	Serial.println("Collecting temperature data.");

	// Reading temperature or humidity takes about 250 milliseconds!
	humidity = dht22.readHumidity();
	// Read temperature as Celsius (the default)
	temperature = dht22.readTemperature();

	// Check if any reads failed and exit early (to try again).
	if (isnan(humidity) || isnan(temperature)) {
		Serial.println("Failed to read from DHT sensor!");
		return DHT_ERR_CHECK;
	}

	Serial.print("Humidity: ");
	Serial.print(humidity);
	Serial.print(" %\t");
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.println(" *C ");

	return DHT_OK;
}

void upload() {
  // Just debug messages
  Serial.print("Sending temperature and humidity : [");
  Serial.print(temperature); Serial.print(",");
  Serial.print(humidity);
  Serial.println("]   -> ");
  
	String t = String(temperature);
	String h = String(humidity);

	// Prepare a JSON payload string
	String payload = "{";
	payload += "\"temperature\":"; payload += t; payload += ",";
	payload += "\"humidity\":"; payload += h;
	payload += "}";

	// Send payload
	char attributes[100];
	payload.toCharArray(attributes, 100);
	client.publish("v1/devices/me/telemetry", attributes);
	Serial.println(attributes);
}

void TFT_print_OneWord(unsigned char &X, unsigned char &Y, unsigned int num, 
		unsigned char ShiftX = 0, unsigned char ShiftY = 0) {
	tft.setCursor(X + ShiftX, Y + ShiftY);
	tft.print(num);
	X = tft.getCursorX() - ShiftX;
	Y = tft.getCursorY() - ShiftY;
	tft.setCursor(X, Y);
}

void TFT_print_OneWord(unsigned char &X, unsigned char &Y, char myString[],
		unsigned char ShiftX = 0, unsigned char ShiftY = 0) {
	tft.setCursor(X + ShiftX, Y + ShiftY);
	tft.print(myString);
	X = tft.getCursorX() - ShiftX;
	Y = tft.getCursorY() - ShiftY;
	tft.setCursor(X, Y);
}

void TFT_print_OneWord(unsigned char &X, unsigned char &Y, const unsigned char *bitmap, 
		int weight, int height, unsigned int color) {
	tft.fillRect(X, Y, weight, height, ST7735_BLACK);
	tft.drawBitmap(X, Y, bitmap, weight, height, color);
	X += weight;
	tft.setCursor(X, Y);
}

const int MonthsCount[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

int Lunar::getLunarMonthDays(int _year, int _monthIndex) {
  if ((lunar200y[_year - 1901] & (0x080000 >> (_monthIndex - 1))) == 0)
		return 29;
	else
		return 30;
}

Lunar::Lunar() {
	syear = 0;
	smonth = 0;
	sdate = 0;
	year = 0;
	month = 0;
	date = 0;
}

Lunar::Lunar(int _year, int _month, int _day) {
	for (; LunarBufferIndex != 0; LunarBufferIndex --) {
		if ((LunarBuffer[LunarBufferIndex].syear == _year) && (LunarBuffer[LunarBufferIndex].smonth == _month)
			&& (LunarBuffer[LunarBufferIndex].sdate == _day)) {
			memcpy(this, &LunarBuffer[LunarBufferIndex], sizeof(Lunar));
			return;
		}
	}

	syear = _year;
	smonth = _month;
	sdate = _day;

	int LunarYearIndex = _year - 1901;
	int SpringSolorMonth = (lunar200y[LunarYearIndex] & 0x60) >> 5;
	int SpringSolordate = lunar200y[LunarYearIndex] & 0x1f;
	
	int SpringFromSolorNew = 0;
	if (SpringSolorMonth == 1)
		SpringFromSolorNew = SpringSolordate - 1;
	else
		SpringFromSolorNew = SpringSolordate + 31 - 1;

	int DaysFromSolorNew = MonthsCount[_month - 1] + _day - 1;
	if ((((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0)) && (_month > 2))
		DaysFromSolorNew ++;

	int ThisYearLeapMonth = (lunar200y[LunarYearIndex] & 0xf00000) >> 20;
	int LastYearLeapMonth = (lunar200y[LunarYearIndex - 1] & 0xf00000) >> 20;
	if (DaysFromSolorNew < SpringFromSolorNew) {
		int DaysFromSpring = SpringFromSolorNew - DaysFromSolorNew;
		int MonthIndex = 0;
		year = _year - 1;
		month = 12;
		bool isPassLeapMonth = false;
		if (LastYearLeapMonth == 0) {
			MonthIndex = 12;
			isPassLeapMonth = true;
		}
		else {
			MonthIndex = 13;
			isPassLeapMonth = false;
		}
				
		for (; 1 != 0; ) {
			int DaysInThisMonth = getLunarMonthDays(year, MonthIndex);
			if (DaysFromSpring > DaysInThisMonth) {
				if ((LastYearLeapMonth == month) && (!isPassLeapMonth))
					isPassLeapMonth = true;
				else
					month--;
				MonthIndex--;
				DaysFromSpring -= DaysInThisMonth;
			}
			else {
				date = DaysInThisMonth - DaysFromSpring + 1;
				if ((LastYearLeapMonth == month) && (!isPassLeapMonth))
					isLeap = true;
				else
					isLeap = false;
				break;
			}
		}
	}
	else {
		int DaysFromSpring = DaysFromSolorNew - SpringFromSolorNew;
		int MonthIndex = 1;
		year = _year;
		month = 1;
		bool isPassLeapMonth = false;
		
		/*Serial.print("SpringFromSolorNew");
		Serial.println(SpringFromSolorNew);
		Serial.print("DaysFromSolorNew:");
		Serial.println(DaysFromSolorNew);
		Serial.print("DaysFromSpring:");
		Serial.println(DaysFromSpring);
		Serial.print("MonthIndex:");
		Serial.println(MonthIndex);
		Serial.print("ThisYearLeapMonth:");
		Serial.println(ThisYearLeapMonth);
		Serial.print("LastYearLeapMonth:");
		Serial.println(LastYearLeapMonth);*/
		for (int i=0 ; 1 != 0; ++i) {
			int DaysInThisMonth = getLunarMonthDays(year, MonthIndex);
			Serial.print("DaysInThisMonth:");
			Serial.println(DaysInThisMonth);
			if (DaysFromSpring >= DaysInThisMonth) {
				if ((ThisYearLeapMonth == month) && (!isPassLeapMonth))
					isPassLeapMonth = true;
				else
					month++;
				MonthIndex++;
				DaysFromSpring -= DaysInThisMonth;
			}
			else {
				date = DaysFromSpring + 1;
				if ((ThisYearLeapMonth == month) && (!isPassLeapMonth))
					isLeap = true;
				else
					isLeap = false;
				break;
			}
			/*Serial.print("DaysFromSpring:");
			Serial.println(DaysFromSpring);*/
		}
	}

	if (LunarBufferIndex < 8)
		LunarBufferIndex++;
	memcpy(&LunarBuffer[LunarBufferIndex], this, sizeof(Lunar));
}

int lyears, lmonths, ldate, lhours, lminutes, lseconds;

void TFT_print() {
	int years = year(), months = month(), days = day(), hours = hour(),
		minutes = minute(), seconds = second(), weekdays = weekday();
	
	//check which part need to be reflash
	bool mustReflash = false;
	if (count == 0)
		mustReflash = true;
	if ((lyears == years) && (lmonths == months) && (ldate == days))
		isDisplay &= 0x3f;
	else
		isDisplay |= 0xc0;
	if (lhours == hours)
		isDisplay &= 0xcf;
	else {
		if ((hours >= 0) && (hours < 10))
			if (lhours >= 10)
				isDisplay |= 0x30;
			else
				isDisplay = ((isDisplay) & 0xdf) | 0x10;
		if ((hours >= 10) && (hours < 24))
			if ((lhours / 10) == (hours / 10))
				isDisplay = ((isDisplay) & 0xdf) | 0x10;
			else
				isDisplay |= 0x30;
	}
	if (lminutes == minutes)
		isDisplay &= 0xf3;
	else {
		if ((minutes >= 0) && (minutes < 10))
			if (lminutes >= 10)
				isDisplay |= 0x0c;
			else
				isDisplay = ((isDisplay) & 0xf7) | 0x04;
		if ((minutes >= 10) && (minutes < 60))
			if ((lminutes / 10) == (minutes / 10))
				isDisplay = ((isDisplay) & 0xf7) | 0x04;
			else
				isDisplay |= 0x0c;
	}
	if (lseconds == seconds)
		isDisplay &= 0xfc;
	else{
		if ((seconds >= 0) && (seconds < 10))
			if (lseconds >= 10)
				isDisplay |= 0x03;
			else
				isDisplay = ((isDisplay) & 0xfd) | 0x01;
		if ((seconds >= 10) && (seconds < 60))
			if ((lseconds / 10) == (seconds / 10))
				isDisplay = ((isDisplay) & 0xfd) | 0x01;
			else
				isDisplay |= 0x03;
	}
	if (count == 0)
		isDisplay = 0xff;
  Serial.print("isDisplay:");
  Serial.println(isDisplay);
  
	if (count == 0) {
		tft.setTextWrap(true);
		tft.fillScreen(ST77XX_BLACK);
		tft.setCursor(0, 30);
		tft.setTextColor(ST77XX_YELLOW);
		tft.setTextSize(1);
	}
	//print time info
	unsigned char NowCursorX = 0;
	unsigned char FirstLineY = 2;
	if (((isDisplay & 0x80) >> 7) || mustReflash) {
		tft.setCursor(NowCursorX, FirstLineY);
		tft.fillRect(NowCursorX, FirstLineY, 160, 12, ST7735_BLACK);
		TFT_print_OneWord(NowCursorX, FirstLineY, years, 0, 2);
		TFT_print_OneWord(NowCursorX, FirstLineY, Word_nian, WordWeight, WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, FirstLineY, months, 0, 2);
		TFT_print_OneWord(NowCursorX, FirstLineY, Word_yue, WordWeight, WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, FirstLineY, days, 0, 2);
		TFT_print_OneWord(NowCursorX, FirstLineY, Word_ri, WordWeight, WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, FirstLineY, "  ", 0, 2);
		TFT_print_OneWord(NowCursorX, FirstLineY, Word_zhou, WordWeight, WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, FirstLineY, Word_xingqi[(weekdays + 5) % 7], WordWeight, WordHeight, ST7735_YELLOW);
	}
	
	//print tiangan dizhi and lunar
	NowCursorX = 0;
	unsigned char SecondLineY = 16;
	Lunar mylunar(years, months, days);

	Serial.print("l_year:");
	Serial.println(mylunar.year);
	Serial.print("l_month:");
	Serial.println(mylunar.month);
	Serial.print("l_date:");
	Serial.println(mylunar.date);
	Serial.print("l_isLeap:");
	Serial.println(mylunar.isLeap);
	int WordOneIndex = (mylunar.year - 1901 + 7) % 10, WordTwoIndex = (mylunar.year - 1901 + 1) % 12;
	if (((isDisplay & 0x40) >> 6) || mustReflash) {
		tft.fillRect(NowCursorX, SecondLineY, 160, 12, ST7735_BLACK);
		TFT_print_OneWord(NowCursorX, FirstLineY, "  ", 0, 2);
		TFT_print_OneWord(NowCursorX, SecondLineY, Word_tian[WordOneIndex], WordWeight,
			WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, SecondLineY, Word_di[WordTwoIndex], WordWeight,
			WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, SecondLineY, Word_nian, WordWeight, WordHeight, ST7735_YELLOW);
		if (mylunar.isLeap == true)
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_run, WordWeight, WordHeight,
				ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[mylunar.month - 1], WordWeight, WordHeight,
			ST7735_YELLOW);
		if (mylunar.month == 11)
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_xingqi[0], WordWeight,
				WordHeight, ST7735_YELLOW);
		TFT_print_OneWord(NowCursorX, SecondLineY, Word_yue, WordWeight,
			WordHeight, ST7735_YELLOW);
		if (mylunar.date > 0 && mylunar.date < 11) {
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_chu, WordWeight,
				WordHeight, ST7735_YELLOW);
			if (mylunar.date == 1)
				TFT_print_OneWord(NowCursorX, SecondLineY, Word_xingqi[1], WordWeight,
					WordHeight, ST7735_YELLOW);
			else
				TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[mylunar.date - 1], WordWeight,
					WordHeight, ST7735_YELLOW);
		}
		else if (mylunar.date > 10 && mylunar.date < 20) {
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[9], WordWeight,
				WordHeight, ST7735_YELLOW);
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[mylunar.date - 11], WordWeight,
				WordHeight, ST7735_YELLOW);
		}
		else if (mylunar.date == 20) {
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_nian1, WordWeight,
				WordHeight, ST7735_YELLOW);
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[9], WordWeight,
				WordHeight, ST7735_YELLOW);
		}
		else if (mylunar.date > 20 && mylunar.date < 30) {
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_nian1, WordWeight,
				WordHeight, ST7735_YELLOW);
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[mylunar.date - 21], WordWeight,
				WordHeight, ST7735_YELLOW);
		}
		else if (mylunar.date == 30) {
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[2], WordWeight,
				WordHeight, ST7735_YELLOW);
			TFT_print_OneWord(NowCursorX, SecondLineY, Word_lunaryue[9], WordWeight,
				WordHeight, ST7735_YELLOW);
		}
	}

	//print real time
	NowCursorX = 32;
	unsigned char ThirdLineY = 30;
	tft.setTextSize(2);
	if (hours < 10) {
		if ((isDisplay & 0x20) >> 5)
			tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK);
		if (((isDisplay & 0x10) >> 4) || mustReflash) {
			NowCursorX += 6;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, hours);
			NowCursorX += 6;
		}
	else
		NowCursorX += 24;
	}
	else if (((isDisplay & 0x20) >> 5) || mustReflash) {
		tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK);
		TFT_print_OneWord(NowCursorX, ThirdLineY, hours);
	}
		else if (((isDisplay & 0x10) >> 4) || mustReflash) {
			NowCursorX += 12;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, hours % 10);
		}
		else
			NowCursorX += 24;
	if (mustReflash)
		TFT_print_OneWord(NowCursorX, ThirdLineY, ":");
	else
		NowCursorX += 12;

	if (minutes < 10) {
		if ((isDisplay & 0x08) >> 3)
			tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK); 
		if ((isDisplay & 0x04) >> 2) {
			NowCursorX += 6;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, minutes);
			NowCursorX += 6;
		}
		else
			NowCursorX += 24;
	}
	else
		if (((isDisplay & 0x08) >> 3) || mustReflash) {
			tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, minutes);
		}
		else if (((isDisplay & 0x04) >> 2) || mustReflash) {
			NowCursorX += 12;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, minutes % 10);
		}
		else
			NowCursorX += 24;
	if (mustReflash)
		TFT_print_OneWord(NowCursorX, ThirdLineY, ":");
	else
		NowCursorX += 12;
	if (seconds < 10) {
		if ((isDisplay & 0x02) >> 1)
			tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK);
		if (isDisplay & 0x01) {
			NowCursorX += 6;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, seconds);
			NowCursorX += 6;
		}
		else
			NowCursorX += 24;
	}
	else
		if (((isDisplay & 0x02) >> 1) || mustReflash) {
			tft.fillRect(NowCursorX, ThirdLineY, 24, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, seconds);
		}
		else if (isDisplay & 0x01) {
			NowCursorX += 12;
			tft.fillRect(NowCursorX, ThirdLineY, 12, 16, ST7735_BLACK);
			TFT_print_OneWord(NowCursorX, ThirdLineY, seconds % 10);
		}
	else
		NowCursorX += 24;
  
	//print sensor info
	NowCursorX = 0;
	unsigned char ForthLineY = 48;
	tft.setCursor(NowCursorX, ForthLineY);
	tft.setTextSize(1);
	if (mustReflash)
		tft.print("Temperature:");
	else {
		NowCursorX += (12 * 6);
	  tft.setCursor(NowCursorX, ForthLineY);
	  tft.fillRect(NowCursorX, ForthLineY, 30, 8, ST7735_BLACK);
	}
	tft.println(temperature);
	NowCursorX = 0;
	if (mustReflash)
		tft.print("Humidity:");
	else {
		NowCursorX += (9 * 6);
	  tft.setCursor(NowCursorX, ForthLineY + 8);
	  tft.fillRect(NowCursorX, ForthLineY + 8, 30, 8, ST7735_BLACK);
  }
	tft.println(humidity);
	Serial.print(count);
	Serial.print(":");
	Serial.println("Have been printed to TFT!");
	Serial.println("============end===============");

  lyears = years;
  lmonths = months;
  ldate = days;
  lhours = hours;
  lminutes = minutes;
  lseconds = seconds;
  count += 1;
}

void sendNTPPacket(IPAddress &address) {
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	packetBuffer[0] = 0b11100011; //LI, Version, Mode
	packetBuffer[1] = 0; // Stratum, or type of clock
	packetBuffer[2] = 6; //Polling Interval
	packetBuffer[3] = 0xEC; //Peer Clock Precision
	//8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	Udp.beginPacket(address, 123);
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}

time_t getNTPTime() {
	IPAddress NTPServerIP;

	// Give up any data received before
	while (Udp.parsePacket() > 0);

	Serial.println("TRansmit NTP REquest");
	//Get ip from host
	WiFi.hostByName(NTPServerName, NTPServerIP);
	Serial.print(NTPServerName);
	Serial.print(":");
	Serial.println(NTPServerIP);
	sendNTPPacket(NTPServerIP);
	unsigned int t_begin = millis();
	while (millis() - t_begin < 1500) {
		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			Udp.read(packetBuffer, NTP_PACKET_SIZE);
			unsigned long secsSince1900;
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			Serial.print("time:");
			Serial.println(secsSince1900);
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}

	Serial.println("No NTP Response!");
	return 0;
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Connecting to Thingsboard node ...");
		// Attempt to connect (clientId, username, password)
		if (client.connect("esp8266 Device", TOKEN, NULL)) {
			Serial.println("[DONE]");
		}
		else {
			Serial.print("[FAILED] [ rc = ");
			Serial.print(client.state());
			Serial.println(" : retrying in 5 seconds]");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void wifiConnect() {
	Serial.print("Connecting WiFi...");
	WiFi.mode(WIFI_STA);
	unsigned long t_old = millis();
	for (int i = 0; true; ) {
		WiFi.begin(ssid[i], password[i]);
		for (int j = 0; j != 10; ++j) {
			if (WiFi.status() != WL_CONNECTED) {
				delay(500);
				Serial.print(".");
			}
			else
				break;
		}
		if ((WiFi.status() != WL_CONNECTED) && ((millis() - t_old) <= WiFiCount*5000)) {
			WiFi.disconnect();
			i = (i + 1) % WiFiCount;
		}
		else
			break;
	}
	Serial.println("");
	Serial.println("WiFi connected!");
	Serial.print("WiFi ssid: ");
	Serial.println(WiFi.SSID());
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void setup() {
	Serial.begin(9600);
	while (!Serial)
		continue;
	//LED setup
	Serial.println("LED setup started!");
	pinMode(LED_PIN, OUTPUT);
	LED_star(1, 500);
	Serial.println("LED setup finished!");

	//DHT22 setup
	Serial.println("DHT22 setup started!");
	dht22.begin();
	//pinMode(DHT_PIN, INPUT);
	//digitalWrite(DHT_PIN, HIGH);
	delay(500);
	LED_star(1, 500);
	Serial.println("DHT22 setup finished!");

	//TFT setup
	Serial.println("TFT setup started!");
	tft.initR(INITR_MINI160x80);
	tft.setRotation(1);
	Serial.print("Width:");
	Serial.println(tft.width());
	Serial.print("Height:");
	Serial.println(tft.height());
	unsigned long t_old = micros();
	tft.fillScreen(ST77XX_BLUE);
	unsigned long t_new = micros();
	Serial.print("TFT fillScreen:");
	Serial.println(t_new - t_old);
	delay(500);
	LED_star(1, 500);
	Serial.println("TFT setup finished!");
	
	//ESP8266 setup
	Serial.println("ESP8266 setup started!");
	wifiConnect();
  Serial.println("ESP8266 setup finished");
  //NTP setup
	Serial.print("Starting UDP: ");
	Udp.begin(myLocalPort);
	Serial.println(Udp.localPort());
	Serial.println("waiting for sync");
	setSyncProvider(getNTPTime);
	setSyncInterval(2);
  while (timeStatus() != timeSet)
    delay(1000);
  setSyncInterval(300);
	delay(500);
  //upload module
  client.setServer( thingsboardServer, 1883 );
}

void loop() {
	// if interval is less than 1000ms, exit
	if ((millis() - LastLoopTime) < 950) {
		delay(50);
		return;
	}
	LastLoopTime = millis();
	unsigned long t_old = millis();
  
	unsigned char isErr = 0;

	int DHT_state = DHT_read();
	if (DHT_state != DHT_OK)
		isErr = DHT_state;
	
	if (timeStatus() != timeNotSet) {
		if (now() != PreviouDisplay)
			PreviouDisplay = now();
		else
			isErr = 3;
	}

	if (!(WiFi.status() == WL_CONNECTED))
		wifiConnect();
	else if (!client.connected()) {
		reconnect();
	}

	if (isErr == 0) {
		upload();
		TFT_print();
	}
	else {
		LED_star(2, 100);
		Serial.print("ERR code:");
		Serial.println(isErr);
	}

	unsigned long t_new = millis();
	Serial.print("time:");
	Serial.println(t_new - t_old);
}
