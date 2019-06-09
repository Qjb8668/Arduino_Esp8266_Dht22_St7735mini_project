# Arduino_Esp8266_Dht22_St7735mini_project
## Introduction
This is a project to use a SingleChip Microcomputer integrated with a wifi module. I also use an TFT monitor to display my well-designed content and a temperature/humidity sensor. It can display today's date, present time and high-precision temperature and humidity now.
## features
1. It can display today's date of solor and lunar calendar.
2. It can display present time which is getting from NTP server.
3. It can display current temperature and humidity getting from Dht22 sensor in real time.
4. The monitor would only reflash the area which is need to be update. It means the monitor won't reflash the whole area each time.
## more details about components
1. ESP8266 with CH340 chip. ESP8266 is originally an simple wifi module with high performence. Some company combine the ESP8266 wifi module and a MCU(micro control system) and become a SingleChip Microcomputer with the help of CH340 chip which make the compuer  control(get Serial information, flash the program, ...) more easily.
2. DHT22 is a high-precision temperature/humidity sensor. Its precison is up to 0.2 degree centigrade of temperture and 0.2% of humidity. More details will be upload to *doc* floder. (***maybe later?***)
3. ST7735 is a type of chip to control plenty of TFT or OLED monitors. My monitor is 0.96 inch TFT monitor with 160*80 pixels. I meet some trouble here because most of information is usually about 1.44 inch monitor or larger. Their code might have fatal incompatibility problem in my mini monitor. I had to read the ST7735 libraries files to get more details. (***Yeah It is really important to learn to read the library source files instead of only using it.***) And I find my mini monitor is called "mini ST7735" in library files. Thus with some changes, I cn control what I wnt to display easily.
## details about source files
Arduino is a good platform to control many kinds of SingleChip Microcomputer and sensors easily. It provides Arduino IDE to write and flash the program to SingleChip Microcomputer easily. Aruduino IDE is based on c++ so that you can use your previous library easily. My project is based on Arduino, too. ***You need to install CH340 driver in Window or MacOS system at first to make Arduino recognise your device.***

Some libraries is used in this project. They are in *lib* floder. Most of them can be download in Arduino IDE while some of them need to be move to Arduino library floder manually.

The source file is seperated to three parts. The firt part is about getting temperature and humidity. The second part is the wifi setting and NTP seeting. It need to connect the wifi at first. You can chenge the wifi ssid and password easily in the files. Once it connects to wifi, it will try to connect NTP server(I use the sjtu NTP server, you can change it to another one) and every 300s(you can change too) will synchronize automatically. It would calculate the year, month, date and change them to lunar calendar. The third part is the monitor. It would flash the whole area to blue in initialization. It has to use the print-bit-pic function to display chinese in the monitor. And it would reflash the area intellengently so that it would decrease the time it costs and more convenient to watch the monitor.
## improvement
1. Maybe there are some bugs need to be fixed.
2. Add a feature that it can display the week today.
3. I thought the Arduino can only compile a file so I push all the codes in a big file. I hope to change the big file into modularization design.