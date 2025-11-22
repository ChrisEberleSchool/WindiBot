#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class WeatherAPI {
public:
    static WeatherAPI& getInstance() {
        static WeatherAPI instance;
        return instance;
    }

    // Fetch weather from Open-Meteo using latitude & longitude
    bool fetchWeather(float lat, float lon);

    // Getters
    float getTemperature() { return temperature; }
    float getWindSpeed() { return windSpeed; }
    int getWeatherCode() { return weatherCode; }

private:
    WeatherAPI() {}
    WeatherAPI(const WeatherAPI&) = delete;
    void operator=(const WeatherAPI&) = delete;

    // Data
    float temperature = 0.0;
    float windSpeed = 0.0;
    int weatherCode = 0;
};

#endif
