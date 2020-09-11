#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

#include "AsyncSimpleWeather.hpp"
#include "config.hpp"
#include "misc.hpp"

// Replace with your wifi credentials

WeatherDto crnt_weatherData = {"Clear", "Not available", CLEAR, -1, 0, 0, 0, 0, 0, 0};
OpenWeather weather(api_key, location_key);

CRGB leds[NUM_LEDS];

void displayRain(double rainPerc) {
    static unsigned long lastRun = 0;
    static unsigned long variableTime = 200;
    if (millis() - lastRun < variableTime) {
        return;
    }
    Serial.printf("Rain Perc: %.3f\n", rainPerc);
    int rainDrop = -1;
    if (random8() > (uint8_t)(255 - 80 * rainPerc))
        rainDrop = random8(COUNT_CLOUD);
    for (size_t i = 0; i < COUNT_CLOUD; i++) {
        leds[START_CLOUD + i] = (i == rainDrop) ? rainColorDrop : rainColorMain;
    }
    lastRun = millis();
}

void displayThunderstorm() {
    static unsigned long lastRun = 0;
    static unsigned long variableTime = 0;
    static uint8_t round = 0;
    static uint8_t thunderBolt_1 = START_CLOUD;
    static uint8_t thunderBolt_2 = START_CLOUD;
    static CRGB igniteLimit = thunderColorBolt / 4;  // Limit color when to ignite the seccond thunderbolt -> 25% of thunderBoltColor -> ignite 2nd one

    if (millis() - lastRun < variableTime) {
        for (size_t i = 0; i < COUNT_CLOUD; i++) {
            if ((i == thunderBolt_1 || i == thunderBolt_2))
                continue;
            if (random(8) < 3)
                leds[START_CLOUD + i] = thunderColorRainDrop;
            else
                leds[START_CLOUD + i] = thunderColorRain;
        }
        return;
    }
    lastRun = millis();
    if (round == 0) {
        thunderBolt_1 = random8(COUNT_CLOUD);
        leds[START_CLOUD + thunderBolt_1] = thunderColorBolt;  // Ignite one thunderbolt
        if (random8() > 230)
            fill_solid(leds, COUNT_BASE, thunderColorBolt);
        variableTime = 50;
        round = 1;
    } else {
        fadeTowardColor(leds[START_CLOUD + thunderBolt_1], thunderColorRain, 40 + random8(30));
        fadeTowardColor(leds[START_CLOUD + thunderBolt_2], thunderColorRain, 30 + random8(20));
        if (leds[START_CLOUD + thunderBolt_1] == thunderColorRain && leds[START_CLOUD + thunderBolt_2] == thunderColorRain) {  // Wait 5 Rounds to fade it to black
            round = 0;
            variableTime = 4000 + random16(1000);
        } else if (leds[START_CLOUD + thunderBolt_1] <= igniteLimit && leds[START_CLOUD + thunderBolt_2] == thunderColorRain) {
            while ((thunderBolt_2 = random8(COUNT_CLOUD)) == thunderBolt_1) {
            };
            if (random8() > 230)
                fill_solid(leds, COUNT_BASE, thunderColorBolt);
            leds[START_CLOUD + thunderBolt_2] = leds[START_CLOUD + thunderBolt_1] = CRGB::White;  // ignite a new thunderbolt and the old again
        }
    }
}

void displaySnow(double snowPerc) {
    static unsigned long lastRun = 0;
    static unsigned long variableTime = 200;
    if (millis() - lastRun < variableTime) {
        return;
    }
    // Serial.printf("Snow Perc: %.3f\n", snowPerc);
    int flakeDrop = -1;
    if (random8() > (uint8_t)(255 - 80 * snowPerc))
        flakeDrop = random8(COUNT_CLOUD);
    for (size_t i = 0; i < COUNT_CLOUD; i++) {
        leds[START_CLOUD + i] = (i == flakeDrop) ? snowColorFlake : snowColorMain;
    }
    lastRun = millis();
}

void displayCloudyness(double cloudyPerc) {
    static unsigned long lastRun = 0;
    static unsigned long variableTime = 20;
    // Serial.printf("Cloudy Perc: %.3f\n", cloudyPerc);
    if (millis() - lastRun < variableTime) {
        return;
    }
    // CRGB sun = CRGB(199, 119, 16);// little bit lighter
    fill_solid(leds + START_CLOUD, COUNT_CLOUD, sunColor);
    fadeToBlackBy(leds + START_CLOUD, COUNT_CLOUD, 230 * cloudyPerc);
    leds[START_CLOUD] = sunColor;
    lastRun = millis();
}

void displayTemperature() {
    static unsigned long lastRun = 0;
    static unsigned long variableTime = 50;
    static uint8_t runCnt = 0;

    if (millis() - lastRun < variableTime)
        return;
    CRGB c = colorFromTemperature(crnt_weatherData.current_Temp);
    CRGB c_off = c;
    c_off.fadeToBlackBy(220);
    for (size_t i = START_BASE; i < COUNT_BASE / 4; i++) {
        fadeTowardColor(leds[START_BASE + i * 4], (runCnt == 0) ? c : c_off, 4);
        fadeTowardColor(leds[START_BASE + i * 4 + 1], (runCnt == 0) ? c_off : c, 4);

        leds[START_BASE + i * 4 + 2] = leds[START_BASE + i * 4 + 1];
        leds[START_BASE + i * 4 + 3] = leds[START_BASE + i * 4];
    }

    if (leds[START_BASE + runCnt] == c && leds[START_BASE + 1 - runCnt] == c_off) {
        runCnt = (runCnt + 1) % 2;
    }
    lastRun = millis();
}

int brightnessFromTime() {
    time_t now = time(nullptr);
    tm* p_tm_now = localtime(&now);
    uint16_t cur = p_tm_now->tm_hour * 60 + p_tm_now->tm_min;

    if (cur >= triggerOn && cur < triggerOff)
        return BRIGHTNESS_NORMAL;
    else
        return BRIGHTNESS_NIGHT;
}

void updateEffectToWeather() {
    displayTemperature();

    switch (crnt_weatherData.condition) {
        case CLEAR:
        case CLOUDS:
            displayCloudyness(((double)crnt_weatherData.clouds) / 100);
            break;

        case RAIN:
        case DRIZZLE:
            displayRain(OpenWeather::rainMMToPercentage(crnt_weatherData.rain));
            break;

        case THUNDERSTORM:
            displayThunderstorm();
            break;

        case SNOW:
            displaySnow(((double)crnt_weatherData.snow) / 100);
            break;

        case FOG:
            //TODO:: Change to foggy
            displayCloudyness(((double)crnt_weatherData.clouds) / 100);
            break;
    };
}

void setup() {
    Serial.begin(115200);
    FastLED.delay(3000);  // 3 second delay for recovery

    WiFi.mode(WIFI_STA);

    ArduinoOTA.setHostname("Weather-LED-Flower-Pot");

    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setDither(true);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, AMP_LIMIT);
    FastLED.show();

    configTime(timezone, daylightOffset, "pool.ntp.org", "time.nist.gov");

    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        FastLED.delay(4000);
        ESP.restart();
    }
    ArduinoOTA.begin();
}

void fetchWeather() {
    weather.updateStatus(&crnt_weatherData);
}

unsigned long stamp = 0;
uint8_t rain = 0;
void loop() {
    if (stamp == 0 || millis() - stamp > 15 * 60 * 1000) {
        fetchWeather();
        stamp = millis();
    }
    updateEffectToWeather();
    FastLED.setBrightness(brightnessFromTime());
    FastLED.show();
    ArduinoOTA.handle();
}