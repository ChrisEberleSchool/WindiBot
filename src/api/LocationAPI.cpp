#include "api/LocationAPI.h"

/**
 * Fetch location data based on IP address.
 * Returns true if successful, false otherwise.
 * Populates city, region, country, lat, lon members.
 * Uses ip-api.com for geolocation.
 * Requires WiFi connection.
 * Uses ArduinoJson for JSON parsing.
 * 
 * Example usage:
 * LocationAPI& loc = LocationAPI::getInstance();
 * if(loc.fetchLocation()) {
 *   Serial.println(loc.getCity());
 * }
 */
bool LocationAPI::fetchLocation() {
    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin("http://ip-api.com/json"); 
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
