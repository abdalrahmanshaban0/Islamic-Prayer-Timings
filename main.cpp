#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
using namespace std;

void setNext(string *prayerTimings) {
  time_t now = time(NULL);
  tm *cur = localtime(&now);
  int cur_val = (cur->tm_min) * 60 + (cur->tm_hour) * 60 * 60 + (cur->tm_sec);
  int comming_val = 0, comming_idx = 0;
  for (int i = 0; i < 6; i++) {
    int secs = stoi(prayerTimings[i].substr(0, 2)) * 3600 +
               stoi(prayerTimings[i].substr(3, 2)) * 60;
    if (i == 0)
      comming_val = secs;
    if (secs > cur_val) {
      comming_idx = i;
      comming_val = secs;
      break;
    }
  }

  int dff = comming_val - cur_val;
  if (dff < 0)
    dff += 24 * 3600;

  while (dff--) {
    ofstream nextFile("/home/abdalrahman/.src/Islamic-Prayer-Timings/next",
                      ios::out | ios::trunc);
    if (nextFile.is_open()) {
      nextFile << comming_idx << endl << dff;
      nextFile.flush();
      nextFile.close();
    } else {
      cerr << "Error opening file next!" << endl;
      exit(1);
    }
    this_thread::sleep_for(1000ms);
  }
}

int main() {
  string country = "Egypt", city = "Assiut";
  string url =
      "curl -L \"https://api.aladhan.com/v1/timingsByCity?city=" + city +
      "&country=" + country +
      "\" > /home/abdalrahman/.src/Islamic-Prayer-Timings/response.json";
  system(url.c_str());

  ifstream file("/home/abdalrahman/.src/Islamic-Prayer-Timings/response.json");

  if (!file.is_open()) {
    cerr << "Error opening file response.json!" << endl;
    return 1;
  }

  nlohmann::json jsonResponse;
  try {
    file >> jsonResponse;
  } catch (const nlohmann::json::parse_error &e) {
    cerr << "Error parsing JSON: " << e.what() << endl;
    return 1;
  }
  file.close();

  auto data = jsonResponse["data"];
  auto timings = data["timings"];

  string prayerTimings[6];
  prayerTimings[0] = timings["Fajr"];
  prayerTimings[1] = timings["Sunrise"];
  prayerTimings[2] = timings["Dhuhr"];
  prayerTimings[3] = timings["Asr"];
  prayerTimings[4] = timings["Maghrib"];
  prayerTimings[5] = timings["Isha"];

  while (true)
    setNext(prayerTimings);

  return 0;
}
