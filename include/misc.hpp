#pragma once
#include <Arduino.h>
#include <FastLED.h>

void nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount);

CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount);

CRGB colorFromTemperature(double temp);