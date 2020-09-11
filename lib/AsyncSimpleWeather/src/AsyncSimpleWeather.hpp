#pragma once

/*
  SimpleWeather.cpp - Library to get weather from Openweather
  Created by Dushyant Ahuja, September 2019.
  https://github.com/dushyantahuja/SimpleWeather
  Modified by Patrick Schmidt for async request!!!
  Released into the public domain.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>

#include "asyncHTTPrequest.h"

enum WeatherCondition {
    CLEAR,
    CLOUDS,
    RAIN,
    DRIZZLE,
    THUNDERSTORM,
    SNOW,
    FOG
};

struct WeatherDto {
    String weather;
    String description;
    WeatherCondition condition;
    int id;
    double current_Temp;
    double min_temp;
    double max_temp;
    int humidity;
    double rain;
    double snow;
    int clouds;
};

class OpenWeather {
   public:
    OpenWeather();
    OpenWeather(String Key, String City);
    OpenWeather(String Key, float lat, float longi);
    OpenWeather(String Key, String City, boolean forecast);
    void updateStatus(WeatherDto *w);
    String getResponse();

    static double rainMMToPercentage(double mm);

   private:
    String _Response;
    String _url;
    boolean _forecast;
    Ticker ticker;
    asyncHTTPrequest aClient;

    WeatherCondition stringToEnum(String s);
    void _onData(WeatherDto *dto, asyncHTTPrequest *, int readyState);
};