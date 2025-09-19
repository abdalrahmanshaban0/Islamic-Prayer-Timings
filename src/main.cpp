#include <condition_variable>
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
condition_variable timingsReady;

auto ErrorInRed = "\033[31m[ERROR] \033[0m";

int main() {
  if (const int chk = loadConfig()) {
    cerr << ErrorInRed << "Loading config" << endl;
    return chk;
  }

  if (!isInternetAvailable()) {
    if (const int chk = loadCachedTimings()) {
      if (chk != 2) {
        cerr << ErrorInRed << "Failed to load cached timings!" << endl;
        return chk;
      }
      waitInternet(30);
    }
  }

  std::thread t(updateWorker);

  {
    std::unique_lock lock(dataMutex);
    timingsReady.wait(lock, [] { return !prayerTimings.empty(); });
  }

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
