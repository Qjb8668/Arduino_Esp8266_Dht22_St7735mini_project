#include "debug.h"

void printConsole(const char *str1, const char *str2) {
	Serial.print(*str1);
	Serial.println(*str2);
}

void printConsole(const char *str, const int num) {
	Serial.print(*str);
	Serial.println(num);
}
