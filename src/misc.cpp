#include "misc.hpp"

#include "gradients.hpp"

// Src: https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount) {
    if (cur == target) return;

    if (cur < target) {
        uint8_t delta = target - cur;
        delta = scale8_video(delta, amount);
        cur += delta;
    } else {
        uint8_t delta = cur - target;
        delta = scale8_video(delta, amount);
        cur -= delta;
    }
}

// Src: https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.

CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount) {
    nblendU8TowardU8(cur.red, target.red, amount);
    nblendU8TowardU8(cur.green, target.green, amount);
    nblendU8TowardU8(cur.blue, target.blue, amount);
    return cur;
}

CRGB colorFromTemperature(double temp) {
    double delta = 0;
    int posTemp = 0;
    if (temp < -20) {
        delta = max(temp + 25, 0.0);
        posTemp = 0 + 14 * (delta / 5.0);
    } else if (temp < -10 && temp >= -20) {
        delta = temp + 20;
        posTemp = 14 + 28 * (delta / 10.0);
    } else if (temp < -5 && temp >= -10) {
        delta = temp + 10;
        posTemp = 42 + 28 * (delta / 5.0);
    } else if (temp < 0 && temp >= -5) {
        delta = temp + 5;
        posTemp = 70 + 29 * (delta / 5.0);
    } else if (temp >= 0 && temp < 15) {
        delta = temp;
        posTemp = 99 + 42 * (delta / 15.0);
    } else if (temp >= 15 && temp < 27) {
        delta = temp - 15;
        posTemp = 141 + 43 * (delta / 13.0);
    } else if (temp >= 27 && temp < 36) {
        delta = temp - 27;
        posTemp = 184 + 42 * (delta / 9.0);
    } else if (temp >= 36) {
        delta = min(temp - 36, 6.0);
        posTemp = 226 + 29 * (delta / 6.0);
    }
    CRGBPalette256 myPal = temperature_gp;
    CRGB c = ColorFromPalette(myPal, posTemp);

    // Serial.printf("Temp %.3f, R %i G %i B %i\n", temp, c.r, c.g, c.b);
    return c;
}