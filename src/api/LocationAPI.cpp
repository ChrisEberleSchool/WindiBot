#include "api/LocationAPI.h"

bool LocationAPI::fetchLocation() {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin("http://ip-api.com/json");  // Free geolocation API
    int httpCode = http.GET();

    if (httpCode != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    // Parse JSON using ArduinoJson
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) return false;

    city    = doc["city"] | "";
    region  = doc["regionName"] | "";
    country = doc["country"] | "";
    lat     = doc["lat"] | 0.0;
    lon     = doc["lon"] | 0.0;

    return true;
}
