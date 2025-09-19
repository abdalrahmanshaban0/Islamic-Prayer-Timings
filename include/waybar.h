#ifndef ISLAMIC_PRAYER_TIMINGS_WAYBAR_H
#define ISLAMIC_PRAYER_TIMINGS_WAYBAR_H

#include <string>
#include <vector>

/**
 * @brief Builds and returns a formatted tooltip string showing the Hijri date
 *        and corresponding Islamic prayer times.
 *
 * Combines the given Hijri date and a list of prayer times into a single,
 * human-readable tooltip string. Each prayer time is printed on its own line,
 * preceded by its name, aligned in columns for readability. Times can be
 * shown in either 24-hour or 12-hour format, depending on the @p hour24 flag.
 *
 * @param[in] prayerTimings  Vector of prayer times as strings
 *                           (order must match the @p names array).
 * @param[in] hijriDate      Hijri date to display at the top of the tooltip.
 * @param[in] names          Array of names corresponding to each prayer
 *                           (e.g., {"Fajr", "Sunrise", "Dhuhr", "Asr",
 *                           "Maghrib", "Isha"}).
 * @param[in] hour24         If `true`, display times in 24-hour format;
 *                           if `false`, convert them to 12-hour format using
 *                           `to12HourFormat()`.
 *
 * @return A formatted tooltip string that starts with the Hijri date
 *         followed by each prayer name and its corresponding time, one per
 * line.
 *
 * @warning The function assumes that @p prayerTimings and @p names have the
 *          same length. Supplying mismatched sizes leads to undefined behavior.
 */
std::string formatTooltip(const std::vector<std::string>& prayerTimings,
                          const std::string& hijriDate, std::string names[],
                          bool hour24);

/**
 * @brief Sends text and tooltip to Waybar in JSON format for a custom module.
 *
 * Constructs a minimal JSON object with two fields:
 *   - `"text"`: the main content shown by the Waybar module
 *   - `"tooltip"`: the text displayed on hover
 *
 * The result is printed to `stdout`, which Waybar reads from.
 *
 * @param text     The main text to display in the Waybar module.
 * @param tooltip  The tooltip content to show when hovering over the module.
 *
 * @note This function flushes the output immediately so Waybar can read it
 *       without delay.
 */
void pipeToWayBar(const std::string& text, const std::string& tooltip);

#endif  // ISLAMIC_PRAYER_TIMINGS_WAYBAR_H
