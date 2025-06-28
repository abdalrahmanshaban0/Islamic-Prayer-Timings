#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "include/prayer_timings.h"
#include "include/utils.h"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

// config variables
std::string country, city, onAdhanScript;
bool hour24;

int main() {
  loadConfig();
  vector<string> prayerTimings = loadCachedTimings();
  string hijriDate;

  if (prayerTimings.empty()) {
    while (!isInternetAvailable()) {
      json ret;
      ret["text"] = "No Internet";
      cout << ret.dump() << endl;
      this_thread::sleep_for(10s);
    }
  }

  if (isInternetAvailable()) {
    pair<vector<string>, string> timings = getTimings();
    prayerTimings = timings.first;
    hijriDate = timings.second;
    if (prayerTimings.empty()) {
      json ret;
      ret["text"] = "Error";
      cout << ret.dump();
      return EXIT_FAILURE;
    }
  }

  while (true) {
    auto [nextPrayIdx, diff] = getNextPrayer(prayerTimings);
    std::string temp;
    if (!hijriDate.empty()) {
      temp += hijriDate + "\n\n";
    }
    for (int i = 0; i < 6; ++i) {
      temp +=
          format("{:<8}:{:>10}{}", names[i],
                 (hour24 ? prayerTimings[i] : to12HourFormat(prayerTimings[i])),
                 i < 5 ? "\n" : "");
    }
    while (diff > 0) {
      json resp;
      resp["text"] = names[nextPrayIdx] + " " + seconds_to_HMS(diff);
      resp["tooltip"] = temp;

      cout << resp.dump() << endl;
      diff--;
      this_thread::sleep_for(1s);
    }

    if (nextPrayIdx != 1) {
      send_prayer_notification(names[nextPrayIdx]);
      runScript(onAdhanScript);
    }
  }
  return EXIT_SUCCESS;
}
