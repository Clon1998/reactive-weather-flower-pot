#include "AsyncSimpleWeather.hpp"

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <WiFiClientSecure.h>
// #define DEBUG
#ifndef DEBUG_PRINT
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif
#endif

OpenWeather::OpenWeather() {
    aClient.setDebug(true);
    aClient.setTimeout(15);
}

OpenWeather::OpenWeather(String Key, String City) : OpenWeather() {
    _forecast = false;
    _url = "/data/2.5/weather?q=" + City + "&appid=" + Key + "&units=metric";
}

OpenWeather::OpenWeather(String Key, float lat, float longi) : OpenWeather() {
    _forecast = false;
    _url = "/data/2.5/weather?lat=" + String(lat) + "&long=" + String(longi) + "&appid=" + Key + "&units=metric";
}

OpenWeather::OpenWeather(String Key, String City, boolean forecast) : OpenWeather() {
    _forecast = forecast;
    if (!forecast)
        _url = "/data/2.5/weather?q=" + City + "&appid=" + Key + "&units=metric&cnt=1";
    else
        _url = "/data/2.5/forecast?q=" + City + "&appid=" + Key + "&units=metric&cnt=2";
}

void OpenWeather::updateStatus(WeatherDto *dto) {
    const char *openweather = "http://api.openweathermap.org";
    //make sure that the client is not busy!
    if (aClient.readyState() == 0 || aClient.readyState() == 4) {
        DEBUG_PRINT("\n-------------\nrequesting URL: ");
        DEBUG_PRINT(openweather + _url);
        aClient.onReadyStateChange([dto](void *obj, asyncHTTPrequest *request, int readyStat) { ((OpenWeather *)(obj))->_onData(dto, request, readyStat); }, this);
        aClient.open("GET", (String(openweather) + _url).c_str());
        aClient.send();
    }
}

void OpenWeather::_onData(WeatherDto *dto, asyncHTTPrequest *request, int readyState) {
    if (readyState == 4) {
        String tmp = request->responseText();
        const size_t capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + 6 * JSON_OBJECT_SIZE(1) + 3 * JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 5 * JSON_OBJECT_SIZE(8) + 570;
        if (tmp.isEmpty()) {
            ticker.once(5, [this, dto]() {
                Serial.println("RETRYYY(IsEmpty)...");
                this->updateStatus(dto);
            });
            return;
        }
        DynamicJsonDocument doc(capacity);
        DeserializationError dErr = deserializeJson(doc, tmp);
        Serial.printf("Deserialization-Code: %s\n", dErr.c_str());
        if (dErr != OK) {
            ticker.once(5, [this, dto]() {
                Serial.println("RETRYYY(Parsing)...");
                this->updateStatus(dto);
            });
            return;
        }

        _Response = tmp;
        DEBUG_PRINT("reply was:");
        DEBUG_PRINT("==========");
        DEBUG_PRINT(_Response);

        if (!_forecast) {
            dto->description = doc["weather"][0]["description"].as<String>();
            dto->weather = doc["weather"][0]["main"].as<String>();
            dto->condition = stringToEnum(dto->weather);

            dto->id = doc["weather"][0]["id"].as<int>();
            dto->current_Temp = doc["main"]["temp"].as<double>();
            dto->min_temp = doc["main"]["temp_min"].as<double>();
            dto->max_temp = doc["main"]["temp_max"].as<double>();
            dto->humidity = doc["main"]["humidity"].as<int>();

            dto->rain = (doc.containsKey("rain")) ? doc["rain"]["1h"].as<double>() : 0;
            dto->clouds = (doc.containsKey("snow")) ? doc["snow"]["1h"].as<double>() : 0;
            dto->clouds = (doc.containsKey("clouds")) ? doc["clouds"]["all"].as<int>() : 0;
        } else {
            // Currently set to get forecast 3 hours from now
            dto->description = doc["list"][1]["weather"][0]["description"].as<String>();
            dto->weather = doc["list"][1]["weather"][0]["main"].as<String>();
            dto->id = doc["list"][1]["weather"]["id"].as<int>();
            dto->current_Temp = doc["list"][1]["main"]["temp"].as<double>();
            dto->min_temp = doc["list"][1]["main"]["temp_min"].as<double>();
            dto->max_temp = doc["list"][1]["main"]["temp_max"].as<double>();
            dto->humidity = doc["list"][1]["main"]["humidity"].as<int>();
            if (dto->id < 700)
                dto->rain = doc["list"][1]["rain"]["3h"].as<double>();
            else
                dto->rain = 0;
        }
        aClient.abort();  //close the connection!
    }
}

double OpenWeather::rainMMToPercentage(double mm) {
    //todo: migrate the linear interpolation to a spline ??
    if (mm < 2.5) {
        // light rain < 2.5 mm
        return (12 * mm) / 100;
    } else if (mm < 7.8) {
        // Moderate rain 2.5 mm (0.098 in) - 7.6 mm (0.30 in)
        return (7.5472 * mm + 11.13) / 100;
    } else if (mm < 50) {
        // Heavy rain < 50mm
        return (0.7109 * mm + 64.455)/100;
    } else {
        // Violent rain > 50mm
        return 1;
    }
}

String OpenWeather::getResponse() {
    return _Response;
}

WeatherCondition OpenWeather::stringToEnum(String s) {
    if (s.equals("Clear"))
        return CLEAR;
    if (s.equals("Clouds"))
        return CLOUDS;
    if (s.equals("Rain"))
        return RAIN;
    if (s.equals("Drizzle"))
        return DRIZZLE;
    if (s.equals("Thunderstorm"))
        return THUNDERSTORM;
    if (s.equals("Snow"))
        return SNOW;
    if (s.equals("Fog"))
        return FOG;
    return CLEAR;  // FALL BACK!!
}