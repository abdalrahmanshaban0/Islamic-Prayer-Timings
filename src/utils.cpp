#include "../include/utils.h"

#include <libnotify/notify.h>

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

string seconds_to_HMS(const int total_seconds) {
  const int hours = total_seconds / 3600;
  const int minutes = (total_seconds % 3600) / 60;
  const int seconds = total_seconds % 60;
  string result = (hours < 10 ? "0" : "") + to_string(hours) + ":" +
                  (minutes < 10 ? "0" : "") + to_string(minutes) + ":" +
                  (seconds < 10 ? "0" : "") + to_string(seconds);
  return result;
}

pair<int, int> getNextPrayer(const vector<string>& prayerTimings) {
  pair<string, string> nextPray;
  time_t now = time(nullptr);
  const tm* cur = localtime(&now);
  int cur_val = (cur->tm_min) * 60 + (cur->tm_hour) * 60 * 60 + (cur->tm_sec);
  int comming_val = 0, comming_idx = 0;
  for (int i = 0; i < 6; i++) {
    int secs = stoi(prayerTimings[i].substr(0, 2)) * 3600 +
               stoi(prayerTimings[i].substr(3, 2)) * 60;
    if (i == 0) comming_val = secs;
    if (secs > cur_val) {
      comming_idx = i;
      comming_val = secs;
      break;
    }
  }
  int diff = comming_val - cur_val;
  if (diff < 0) diff += 24 * 3600;
  return {comming_idx, diff};
}

int send_prayer_notification(const string& prayer_name) {
  if (!notify_init("Islamic Prayer Notification")) {
    cerr << "Failed to initialize libnotify!" << endl;
    return 1;
  }
  // Create a new notification
  NotifyNotification* notification = notify_notification_new(
      "Islamic Prayer Timings", ("It's time for " + prayer_name).c_str(),
      "dialog-information");

  // Display the notification
  if (!notify_notification_show(notification, nullptr)) {
    cerr << "Failed to display notification!" << endl;
    return 2;
  }
  // Free the notification object
  g_object_unref(G_OBJECT(notification));
  // Shutdown libnotify
  notify_uninit();
  return 0;
}

std::string to12HourFormat(const std::string& time24) {
  int hour, minute;
  char colon;

  std::istringstream iss(time24);
  iss >> hour >> colon >> minute;

  std::ostringstream oss;
  bool isPM = hour >= 12;
  int hour12 = hour % 12;
  if (hour12 == 0) hour12 = 12;

  oss << std::setw(2) << std::setfill('0') << hour12 << ":" << std::setw(2)
      << std::setfill('0') << minute << (isPM ? " PM" : " AM");

  return oss.str();
}

void runScript(const std::string& scriptPath) {
  if (fs::exists(scriptPath) && fs::is_regular_file(scriptPath) &&
      access(scriptPath.c_str(), X_OK) == 0) {
    int result = system((scriptPath + " &").c_str());
    if (result != 0) {
      cerr << "Script failed with exit code: " << result << endl;
    }
  } else {
    cerr << "Adhan script not found: " << scriptPath << endl;
  }
}

int secondsUntilMidnight() {
  using namespace std::chrono;

  auto now = system_clock::now();
  time_t now_c = system_clock::to_time_t(now);
  std::tm* now_tm = std::localtime(&now_c);

  now_tm->tm_hour = 0;
  now_tm->tm_min = 0;
  now_tm->tm_sec = 0;
  now_tm->tm_mday += 1;  // move to next day

  auto midnight = system_clock::from_time_t(std::mktime(now_tm));
  auto diff = duration_cast<seconds>(midnight - now);
  return static_cast<int>(diff.count());
}
