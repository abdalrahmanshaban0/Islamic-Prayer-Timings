#ifndef PRAYER_TIMINGS_H
#define PRAYER_TIMINGS_H

#include <string>
#include <vector>

inline std::string names[] = {"Fajr", "Sunrise", "Dhuhr",
                              "Asr",  "Maghrib", "Isha"};

/**
 * Load the config ~/.config/IslamicPrayerTimings/config, if it doesn't exist
 * config-example will be created and loaded
 * @returns 0 on success and other positive value indicates an error
 */
int loadConfig();

/**
 * Gets prayer timings from the cached json response if it's found in
 * ~/.local/share/IslamicPrayerTimings/timings.json and assigns it to the
 * prayerTimings shared variable. This is useful when opening the system without
 * internet (load most recent timings).
 * @returns 0 on success and a positive integer on failure
 */
int loadCachedTimings();

/**
 * @brief Fetches Islamic prayer timings and the current Hijri date.
 *
 * Sends an HTTP GET request to the Aladhan API to retrieve today's prayer
 * times and Hijri date. The response is cached in
 * `~/.local/IslamicPrayerTimings/timings.json` so that it can be reused if
 * there is no internet connection on subsequent calls.
 *
 * @param[out] timings  A vector that will be populated with the retrieved
 *                      prayer times (in order: Fajr, Sunrise, Dhuhr, Asr,
 *                      Maghrib, Isha).
 * @param[out] hdate    A string that will be set to the corresponding Hijri
 * date.
 *
 * @return 0 on success, or a positive integer error code on failure
 *         (e.g., network error, parse error, or cache read failure).
 */
int getTimings(std::vector<std::string>& timings, std::string& hdate);

/**
 * @brief Worker thread that keeps Islamic prayer timings and Hijri date up to
 * date.
 *
 * Runs an endless loop:
 *  - Checks for internet connectivity (waits/retries if unavailable).
 *  - Calls getTimings() to update shared data (prayerTimings, hijriDate,
 * waybarTooltip).
 *  - Sleeps until a second past midnight, then repeats.
 *
 * @param[in] daemon A boolean that determines if timings get updated once or
 * periodically
 * This function never returns.
 */
void updateWorker(bool daemon);

#endif
