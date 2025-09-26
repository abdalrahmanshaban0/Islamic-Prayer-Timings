#include <iostream>
#include <ostream>
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

inline void printUsage() { cout << "Usage: " << argv0 << " [-d|--daemon]\n"; }

auto ErrorInRed = "\033[31m[ERROR] \033[0m";

int main(const int argc, char* argv[]) {
  argv0 = argv[0];
  bool daemon = false;

  if (argc > 2) {
    printUsage();
    return 1;
  }
  if (argc == 2) {
    if (const string arg(argv[1]); arg == "-d" || arg == "--daemon") {
      daemon = true;
    } else {
      printUsage();
      return 0;
    }
  }

  if (const int chk = loadConfig()) {
    cerr << ErrorInRed << "Loading config" << endl;
    return chk;
  }

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
