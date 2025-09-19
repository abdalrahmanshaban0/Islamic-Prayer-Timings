#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "../include/utils.h"

using namespace std;

string formatTooltip(const vector<string>& prayerTimings, const string& hijriDate,
                   string names[6], const bool hour24) {
  string tooltip = hijriDate + "\n\n";
  for (int i = 0; i < prayerTimings.size(); ++i) {
    ostringstream ss;
    ss << left << setw(10) << names[i] << setw(5) << ":" << right << setw(8)
       << (hour24 ? prayerTimings[i] : to12HourFormat(prayerTimings[i]))
       << (i+1 < prayerTimings.size() ? "\n" : "");

    tooltip += ss.str();
  }
  return tooltip;
}

void pipeToWayBar(const std::string& text, const std::string& tooltip) {
  using nlohmann::json;
  const json payload = {{"text", text}, {"tooltip", tooltip}};
  std::cout << payload.dump() << std::endl;
}