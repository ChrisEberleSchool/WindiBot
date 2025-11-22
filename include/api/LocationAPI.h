#ifndef LOCATION_API_H
#define LOCATION_API_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class LocationAPI {
public:
    static LocationAPI& getInstance() {
        static LocationAPI instance;
        return instance;
    }

    // Fetch location based on the device's current public IP
    bool fetchLocation();

    // Getters
    String getCity() { return city; }
    String getRegion() { return region; }
    String getCountry() { return country; }
    float getLat() { return lat; }
    float getLon() { return lon; }

private:
    LocationAPI() {}                             // private constructor
    LocationAPI(const LocationAPI&) = delete;        // disable copy
    void operator=(const LocationAPI&) = delete; // disable assignment

    String city = "";
    String region = "";
    String country = "";
    float lat = 0.0;
    float lon = 0.0;
};

#endif
