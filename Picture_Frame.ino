#include <Adafruit_SSD1306.h>
#include <Boards.h>
#include <Firmata.h>
#include <FirmataConstants.h>
#include <FirmataDefines.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DS3231.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Offset of first date (2/17/2019) to UNIX origin (1/1/1970)
// https://planetcalc.com/273/
// 17,944 days
#define UNIXOFFSET 17944

#define LOW_BATT_PIN     LED_BUILTIN
#define EXT_LIGHTS_PIN   1
#define BATT_VOLTAGE     A0

#define BATT_THRESHOLD   450               // 2.2 / 5 * 1024 - 1 ~= 450

void updateTime(void);
void setupScreenA(void);
void setupScreenB(void);
void displayYears(void);
void displayDateTime(void);
void displayDays(void);
void displayAnniversary(void);
void updateScreen(bool clearPrevious);


RTClib RTC;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char screenBuffer[40];
int  year;
int  month;
int  day;
int  hour;
int  minute;
int  second;
long unixTime;
double yearsDating;

unsigned int batteryCounts;

void setup() {
  Serial.begin(9600);
  
  pinMode(LOW_BATT_PIN, OUTPUT);
  pinMode(EXT_LIGHTS_PIN, OUTPUT);
  pinMode(BATT_VOLTAGE, INPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  delay(500);
}


void loop() 
{    
  updateTime();
  displayDateTime();
  updateScreen(false);
  delay(10000);
  
  displayDays();
  updateScreen(true);
  delay(3000);
  
  displayYears();
  updateScreen(true);
  delay(3000);

  batteryCounts = analogRead(BATT_VOLTAGE);
  batteryCounts += analogRead(BATT_VOLTAGE);
  batteryCounts += analogRead(BATT_VOLTAGE);
  batteryCounts += analogRead(BATT_VOLTAGE);
  batteryCounts /= 4;

  if(batteryCounts <= BATT_THRESHOLD)
  {
    digitalWrite(LOW_BATT_PIN, HIGH);
  }
  else
  {
    digitalWrite(LOW_BATT_PIN, LOW);
  }

  if((day == 17) && (month == 2))
  {
    digitalWrite(EXT_LIGHTS_PIN, HIGH);
  }
  else
  {
    digitalWrite(EXT_LIGHTS_PIN, LOW);
  }
}


void updateTime(void)
{
  DateTime now = RTC.now();
  year = now.year();
  month = now.month();
  day = now.day();
  hour = now.hour();
  if(hour > 12)
  {
    hour -=12;
  }
  else if(hour == 0)
  {
	hour = 12;
  }
  minute = now.minute();
  second = now.second();
  unixTime = now.unixtime();
  unixTime = unixTime / 86400;
  unixTime = unixTime - UNIXOFFSET;
  yearsDating = (double(unixTime) / 365);
}

void setupScreenA(void)
{
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(false);
  display.setCursor(5, 9);     // Extra pixels 8 horizontal 16 vertical, split evenly plus 1 to each based on how it looked
  sprintf(screenBuffer, "                                        ");
  sprintf(screenBuffer, "%02d/%02d/%04d\r\n %02d:%02d:%02d\r\n %d days", month, day, year, hour, minute, second, unixTime);
}

void setupScreenB(void)
{
  display.setTextSize(4);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(false);
  display.setCursor(6, 19);
  sprintf(screenBuffer, "                                        ");
  sprintf(screenBuffer, "%02d:%02d", hour, minute);
}

void displayYears(void)
{
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(false);
  display.setCursor(5, 9);
  char str_temp[10];
  sprintf(screenBuffer, "                                        ");
  dtostrf(yearsDating, 6, 3, str_temp);
  sprintf(screenBuffer, "\r\n%s yrs", str_temp);
}

void displayDateTime(void)
{
  display.clearDisplay();
  display.setTextSize(4);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(false);
  display.setCursor(6, 5);
  
  sprintf(screenBuffer, "                                        ");
  sprintf(screenBuffer, "%02d:%02d", hour, minute);
  updateScreen(false);

  display.setTextSize(2);
  display.setCursor(6, 45);
  sprintf(screenBuffer, "                                        ");
  sprintf(screenBuffer, "%02d/%02d/%02d", month, day, year);
}

void displayDays(void)
{
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(false);
  display.setCursor(5, 9);
  char str_temp[10];
  sprintf(screenBuffer, "                                        ");
  dtostrf(unixTime, 5, 0, str_temp);
  sprintf(screenBuffer, "\r\n%s days", str_temp);
}

void displayAnniversary (void)
{
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.invertDisplay(true);
  display.setCursor(5, 9);
  char str_temp[10];
  sprintf(screenBuffer, "                                        ");
  sprintf(screenBuffer, "\r\nHappy Annivesrary!!");
}


void updateScreen(bool clearPrevious) 
{
  if(clearPrevious)
  {
    display.clearDisplay();
  }

  for(int16_t i=0; i<sizeof(screenBuffer); i++) 
  {
    display.write(screenBuffer[i]);
  }
  
  display.display();
}
