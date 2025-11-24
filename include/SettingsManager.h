// SettingsManager.h
#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <Preferences.h>

class SettingsManager {
public:
    static SettingsManager& getInstance() {
        static SettingsManager instance;
        return instance;
    }

    // WEATHER GUARD PREFS
    void saveWeatherGuard(bool isOn) {
        prefs.begin("settings", false);
        prefs.putBool("weatherGuard", isOn);
        prefs.end();
    }
    bool loadWeatherGuard() {
        prefs.begin("settings", true);
        bool open = prefs.getBool("weatherGuard", true);
        prefs.end();
        return open;
    }

    // UNITS PREFS
    void saveUnits(bool isMetric) {
        prefs.begin("settings", false);
        prefs.putBool("useMetric", isMetric);
        prefs.end();
    }
    bool loadUnits() {
        prefs.begin("settings", true);
        bool open = prefs.getBool("useMetric", true);
        prefs.end();
        return open;
    }

    // OFFLINE MODE PREFS
    void saveOfflineMode(bool isOffline) {
        prefs.begin("settings", false);
        prefs.putBool("isOffline", isOffline);
        prefs.end();
    }
    bool loadOfflineMode() {
        prefs.begin("settings", true);
        bool open = prefs.getBool("isOffline", true);
        prefs.end();
        return open;
    }

private:
    SettingsManager() {}
    SettingsManager(const SettingsManager&) = delete;
    void operator=(const SettingsManager&) = delete;

    Preferences prefs;
};

#endif
