#ifndef PRAYER_TIMINGS_H
#define PRAYER_TIMINGS_H

#include <string>
#include <vector>

const std::string names[6] = {"الفجر", "الشروق", "الظهر",
                              "العصر", "المغرب", "العشاء"};

// config variables
extern std::string country, city, onAdhanScript;
extern bool hour24;

/**
 * Load the config ~/.config/IslamicPrayerTimings/config, If doesn't exist
 * config-example will be created and loaded
 */
void loadConfig();

/**
 * Gets prayer timings from the cached json response if it's found in
 * ~/.local/share/IslamicPrayerTimings/timings.json. This is useful when
 * opening the system without internet (load most recent timings).
 * @return vector of cached prayer timings
 */
std::vector<std::string> loadCachedTimings();

/**
 * Get request to aladhan API and store response in
 * ~/.local/IslamicPrayerTimings/timings.json to read them in the next time in
 * case there's no internet.
 * @return pair of prayer timings and hijri date
 */
std::pair<std::vector<std::string>, std::string> getTimings();

#endif
