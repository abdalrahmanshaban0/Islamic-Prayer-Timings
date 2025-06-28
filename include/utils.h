#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

/**
 * Convert seconds to HH:MM:SS format
 * @param total_seconds
 * @return time in HH:MM:SS format
 */
std::string seconds_to_HMS(int total_seconds);

/**
 * Get the next prayer timing index and countdown until
 * @param prayerTimings vector of prayer timings of the day
 * @return Coming pray index and countdown until in seconds
 */
std::pair<int, int> getNextPrayer(
    const std::vector<std::string>& prayerTimings);

/**
 * Checks if you're connected to internet or not
 * @return true if online, false if offline
 */
bool isInternetAvailable();

/**
 * Sends notification using libnotify for the current prayer provided
 * with prayer_name
 * @param prayer_name current prayer timing
 */
void send_prayer_notification(const std::string& prayer_name);

/**
 * converts time from 24-hour format to 12-hour format
 * @param time24 time in 24-hour format
 * @return time in 12-hour format
 */
std::string to12HourFormat(const std::string& time24);

/**
 * Runs a script at the given path.
 * @param scriptPath the absolute path of the script, if the path starts with
 * ~ will be handled.
 */
void runScript(const std::string& scriptPath);

#endif
