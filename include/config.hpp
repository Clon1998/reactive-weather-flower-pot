#pragma once
#include <Arduino.h>
#include <FastLED.h>

//..............................................................................
//.....CCCCCCCCCC...................................ffffffiiiiii................
//....CCCCCCCCCCCC.................................fffffffiiiiii................
//...CCCCCCCCCCCCCC................................fffffffiiiiii................
//..CCCCCCCCCCCCCCCC...............................fffff........................
//..CCCCCC....CCCCC....ooooooooo....nnnnnnnnnnn....fffff...iiiii...ggggggggggg..
//..CCCCC.............ooooooooooo...nnnnnnnnnnnn.nffffffff.iiiii..gggggggggggg..
//.CCCCCC.............oooooooooooo..nnnnnnnnnnnnnnffffffff.iiiii.ggggggggggggg..
//.CCCCC.............ooooooooooooo..nnnnnnnnnnnnn..fffff...iiiii.ggggggggggggg..
//.CCCCC.............ooooo...oooooo.nnnnn...nnnnn..fffff...iiiii.ggggg...ggggg..
//.CCCCC.............ooooo....ooooo.nnnnn...nnnnn..fffff...iiiii.ggggg...ggggg..
//.CCCCCC............oooo.....ooooo.nnnnn...nnnnn..fffff...iiiii.gggg....ggggg..
//..CCCCC............ooooo....ooooo.nnnnn...nnnnn..fffff...iiiii.ggggg...ggggg..
//..CCCCCC....CCCCCC.ooooo...oooooo.nnnnn...nnnnn..fffff...iiiii.ggggg...ggggg..
//..CCCCCCCCCCCCCCCC.ooooooooooooo..nnnnn...nnnnn..fffff...iiiii.ggggggggggggg..
//...CCCCCCCCCCCCCC..ooooooooooooo..nnnnn...nnnnn..fffff...iiiii.ggggggggggggg..
//....CCCCCCCCCCCCC...ooooooooooo...nnnnn...nnnnn..fffff...iiiii..gggggggggggg..
//.....CCCCCCCCCC......ooooooooo....nnnnn...nnnnn..fffff...iiiii...ggggggggggg..
//.......................................................................ggggg..
//...............................................................gggggg.gggggg..
//...............................................................ggggggggggggg..
//................................................................ggggggggggg...
//................................................................gggggggggg....
//..............................................................................

#define DATA_PIN 14  //D5
//#define CLK_PIN   4
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 22
#define BRIGHTNESS_NORMAL 255  // Brightness to display between turnOn -> turnOff
#define BRIGHTNESS_NIGHT 30    // Brightness to display between turnOff -> turnOn
#define AMP_LIMIT 1000         // 1 A=1000 mA

#define START_BASE 0   // Inclusive
#define COUNT_BASE 16  // Exclusive

#define START_CLOUD 16  // Inclusive
#define COUNT_CLOUD 6   // Exclusive

const char* ssid = "XXXXXX";
const char* password = "XXXXXXXXXXX";

const String api_key = "0eeaf533a5ecc18c6c41ffb6867e96ed";
const String location_key = "Mühltal,DE";

const int timezone = 1 * 3600;
const int daylightOffset = 3600;

// CARE! This is coded to expect on the be earlier than off!!!
const int turnOnHr = 7;    // in Hr
const int turnOnMin = 0;   // in min
const int turnOffHr = 22;  // in Hr
const int turnOffMin = 0;  // in min

//..............................................................................
//....CCCCCCC......OOOOOOO.....LLLL.........OOOOOOO.....RRRRRRRRRR..............
//...CCCCCCCCC....OOOOOOOOOO...LLLL........OOOOOOOOOO...RRRRRRRRRRR.............
//..CCCCCCCCCCC..OOOOOOOOOOOO..LLLL.......OOOOOOOOOOOO..RRRRRRRRRRR.............
//..CCCC...CCCCC.OOOOO..OOOOO..LLLL.......OOOOO..OOOOO..RRRR...RRRRR.sssssss....
//.CCCC.....CCC.OOOOO....OOOOO.LLLL......OOOOO....OOOOO.RRRR...RRRRRsssssssss...
//.CCCC.........OOOO......OOOO.LLLL......OOOO......OOOO.RRRRRRRRRRR.ssss.ssss...
//.CCCC.........OOOO......OOOO.LLLL......OOOO......OOOO.RRRRRRRRRRR.sssss.......
//.CCCC.........OOOO......OOOO.LLLL......OOOO......OOOO.RRRRRRRR.....ssssss.....
//.CCCC.....CCC.OOOOO....OOOOO.LLLL......OOOOO....OOOOO.RRRR.RRRR.....sssssss...
//..CCCC...CCCCC.OOOOO..OOOOO..LLLL.......OOOOO..OOOOO..RRRR..RRRR........ssss..
//..CCCCCCCCCCC..OOOOOOOOOOOO..LLLLLLLLLL.OOOOOOOOOOOO..RRRR..RRRRR.ssss..ssss..
//...CCCCCCCCCC...OOOOOOOOOO...LLLLLLLLLL..OOOOOOOOOO...RRRR...RRRRRsssssssss...
//....CCCCCCC.......OOOOOO.....LLLLLLLLLL....OOOOOO.....RRRR....RRRR..ssssss....
//..............................................................................

const CRGB rainColorMain = CRGB(10, 16, 200);
const CRGB rainColorDrop = CRGB(20, 26, 131);

const CRGB snowColorMain = CRGB(150, 150, 150);
const CRGB snowColorFlake = CRGB(200, 200, 200);
const CRGB thunderColorRain = CRGB(0x111111);
const CRGB thunderColorRainDrop = CRGB(0x0F0F0F);
const CRGB thunderColorBolt = CRGB::White;

const CRGB sunColor = CRGB(255, 144, 0);

///////////////////////////////////
// DO NOT EDIT!!!!!
const uint16_t triggerOn = turnOnHr * 60 + turnOnMin;
const uint16_t triggerOff = turnOffHr * 60 + turnOffMin;