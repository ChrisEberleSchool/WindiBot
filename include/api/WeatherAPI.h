#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <api/LocationAPI.h>

class WeatherAPI {
public:
    static WeatherAPI& getInstance() {
        static WeatherAPI instance;
        return instance;
    }

    // Fetch weather from Open-Meteo using latitude & longitude
    bool fetchWeather(float lat, float lon);
    void update() {
        fetchWeather(LocationAPI::getInstance().getLat(), LocationAPI::getInstance().getLon());
    }

    // Getters
    float getTemperature(bool isMetric = true) const {
        return isMetric ? temperature : cToF(temperature);
    }

    float getWindSpeed(bool isMetric = true) const {
        return isMetric ? windSpeed : mpsToMph(windSpeed);
    }
    int getWeatherCode() { return weatherCode; }

    bool isBadWeather();

    String getWeatherDescription() const;

private:
    WeatherAPI() {}
    WeatherAPI(const WeatherAPI&) = delete;
    void operator=(const WeatherAPI&) = delete;

    // Data
    float temperature = 0.0;
    float windSpeed = 0.0;
    int weatherCode = 0;

    // Helpers
    float cToF(float tempC) const { return tempC * 9.0 / 5.0 + 32.0; }
    float mpsToMph(float speedMps) const { return speedMps * 2.23694f; }
};

#endif
