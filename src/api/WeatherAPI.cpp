#include "api/WeatherAPI.h"

bool WeatherAPI::fetchWeather(float lat, float lon) {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;

    // Build API URL
    String url = "https://api.open-meteo.com/v1/forecast?latitude=" +
                 String(lat, 6) +
                 "&longitude=" +
                 String(lon, 6) +
                 "&current_weather=true";

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) return false;

    // Parse weather values
    temperature = doc["current_weather"]["temperature"] | 0.0;
    windSpeed   = doc["current_weather"]["windspeed"]   | 0.0;
    weatherCode = doc["current_weather"]["weathercode"] | 0;

    return true;
}

bool WeatherAPI::isRaining() {
    switch (weatherCode) {
        case 51: case 53: case 55: 
        case 61: case 63: case 65:  
        case 66: case 67:           
        case 80: case 81: case 82:  
        case 95: case 96: case 99:  
            return true;
        default:
            return false;
    }
}

bool WeatherAPI::isSnowing() {
    switch (weatherCode) {
        case 71: case 73: case 75: 
        case 77:                 
        case 85: case 86:        
            return true;
        default:
            return false;
    }
}