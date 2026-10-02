#include <cstring>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <thread>
#include "../include/prayer_timings.h"
#include "../include/utils.h"
#include "../include/waybar.h"

using namespace std;

// Shared data
vector<string> prayerTimings;
string waybarTooltip;
string onAdhanScript;

mutex dataMutex;

const char* argv0;

inline void printUsage() {
  cout << "Usage: " << argv0
       << " [-d|--daemon] [--lat <latitude> --long <longitude>]\n";
}

auto ErrorInRed = "\033[31m[ERROR] \033[0m";

int main(const int argc, char* argv[]) {
  argv0 = argv[0];
  bool daemon = false;
  optional<double> lat, lon;

  for (int i = 1; i < argc; ++i) {
    const string arg = argv[i];
    if (arg == "-d" || arg == "--daemon") {
      daemon = true;
    } else if (arg == "--lat" || arg == "--long") {
      if (i + 1 >= argc) {
        printUsage();
        return 1;
      }
      const char* value = argv[++i];
      try {
        size_t pos = 0;
        const double v = stod(value, &pos);
        if (pos != strlen(value)) throw invalid_argument("trailing chars");
        (arg == "--lat" ? lat : lon) = v;
      } catch (const exception&) {
        cerr << ErrorInRed << "Invalid value for " << arg << ": " << value
             << endl;
        return 1;
      }
    } else {
      printUsage();
      return 1;
    }
  }

  if (lat.has_value() != lon.has_value()) {
    cerr << ErrorInRed << "--lat and --long must be used together" << endl;
    return 1;
  }
  if (lat && (*lat < -90 || *lat > 90 || *lon < -180 || *lon > 180)) {
    cerr << ErrorInRed << "Latitude must be in [-90, 90] and longitude in "
                          "[-180, 180]" << endl;
    return 1;
  }

  if (const int chk = loadConfig()) {
    cerr << ErrorInRed << "Loading config" << endl;
    return chk;
  }

  // CLI coordinates override the config (and city/country)
  if (lat) setCoordinates(*lat, *lon);

  updateWorker(false);

  if (!daemon) {
    auto [nextPrayIdx, diff] = getNextPrayer(prayerTimings);
    const string nextPrayer = names[nextPrayIdx] + " " + seconds_to_HMS(diff);
    cout << "⠀⠀⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠀⠀\n"
            "⠀⣰⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣆⠀\n"
            "⠠⠿⠿⠿⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠿⠿⠄\n"
            "⢰⣶⣶⣶⡄⠀⠀⠀⠀⣀⣴⣾⣿⣿⣷⣦⣀⠀⠀⠀⠀⢠⣶⣶⣶⡆\n"
            "⢸⣿⣿⣿⡇⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⢰⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⡆⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⠟⠁⠈⠻⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⢸⣿⣿⣿⣿⡏⠀⠀⠀⠀⢹⣿⣿⣿⣿⡇⢸⣿⣿⣿⡇\n"
            "⢸⣿⣿⣿⡇⢸⣿⣿⣿⣿⡇⠀⠀⠀⠀⢸⣿⣿⣿⣿⡇⢸⣿⣿⣿⡇\n"
            "⠸⠿⠿⠿⠇⠸⠿⠿⠿⠿⠇⠀⠀⠀⠀⠸⠿⠿⠿⠿⠇⠸⠿⠿⠿⠇\n";

    cout << nextPrayer << '\n';
    cout << waybarTooltip << endl;
    return 0;
  }

  std::thread t(updateWorker, true);
  t.detach();

  while (true) {
    pair<int, int> p;
    {
      std::lock_guard lock(dataMutex);
      p = getNextPrayer(prayerTimings);
    }

    int nextPrayIdx = p.first;
    int diff = p.second;

    while (diff > 0) {
      string waybarText = names[nextPrayIdx] + " " + seconds_to_HMS(diff);
      pipeToWayBar(waybarText, waybarTooltip);
      diff--;
      this_thread::sleep_for(1s);
    }

    if (nextPrayIdx != 1) {
      if (send_prayer_notification(names[nextPrayIdx])) {
        cerr << ErrorInRed << "Failed to send notification!" << endl;
      }
      runScript(onAdhanScript);
    }

    this_thread::sleep_for(1s);
  }
}
