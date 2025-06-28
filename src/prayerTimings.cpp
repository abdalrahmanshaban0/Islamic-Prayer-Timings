#include <curl/curl.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/prayer_timings.h"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

size_t WriteCallback(void* contents, const size_t size, const size_t nmemb,
                     string* output) {
  const size_t totalSize = size * nmemb;
  output->append(static_cast<char*>(contents), totalSize);
  return totalSize;
}

pair<vector<string>, string> getTimings() {
  string readBuffer, hijiriDate;
  vector<string> prayerTimings;

  if (CURL* curl = curl_easy_init()) {
    const string url = "https://api.aladhan.com/v1/timingsByCity?city=" + city +
                       "&country=" + country;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      cerr << "Request failed: " << curl_easy_strerror(res) << "\n";
    } else {
      try {
        json j = json::parse(readBuffer);

        const string home = getenv("HOME");
        const string dirPath = home + "/.local/share/IslamicPrayerTimings";
        const string filePath = dirPath + "/timings.json";

        fs::create_directories(dirPath);
        ofstream outFile(filePath);
        outFile << j.dump(2);
        outFile.close();

        prayerTimings.push_back(j["data"]["timings"]["Fajr"]);
        prayerTimings.push_back(j["data"]["timings"]["Sunrise"]);
        prayerTimings.push_back(j["data"]["timings"]["Dhuhr"]);
        prayerTimings.push_back(j["data"]["timings"]["Asr"]);
        prayerTimings.push_back(j["data"]["timings"]["Maghrib"]);
        prayerTimings.push_back(j["data"]["timings"]["Isha"]);

        // Extract Hijri fields
        string hijri_day = j["data"]["date"]["hijri"]["weekday"]["ar"];
        string hijri_day_no = j["data"]["date"]["hijri"]["day"];
        string hijri_month = j["data"]["date"]["hijri"]["month"]["ar"];
        string hijri_year = j["data"]["date"]["hijri"]["year"];
        hijiriDate = hijri_day + " " + hijri_day_no + " " + hijri_month + " " +
                     hijri_year + " هـ";

      } catch (const exception& e) {
        cerr << "JSON parse error: " << e.what() << '\n';
      }
    }
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return {prayerTimings, hijiriDate};
}

vector<string> loadCachedTimings() {
  vector<string> prayerTimings;
  const string home = getenv("HOME");
  const string filePath =
      fs::path(home + "/.local/share/IslamicPrayerTimings/timings.json");
  if (fs::exists(filePath)) {
    ifstream inFile(filePath);
    json j;
    inFile >> j;

    prayerTimings.push_back(j["data"]["timings"]["Fajr"]);
    prayerTimings.push_back(j["data"]["timings"]["Sunrise"]);
    prayerTimings.push_back(j["data"]["timings"]["Dhuhr"]);
    prayerTimings.push_back(j["data"]["timings"]["Asr"]);
    prayerTimings.push_back(j["data"]["timings"]["Maghrib"]);
    prayerTimings.push_back(j["data"]["timings"]["Isha"]);
  }
  return prayerTimings;
}

void loadConfig() {
  // Expand HOME path
  std::string configDir =
      std::string(getenv("HOME")) + "/.config/IslamicPrayerTimings";
  std::string configPath = configDir + "/config";

  // If config does not exist, create it with default values
  if (!fs::exists(configPath)) {
    fs::create_directories(configDir);  // ensure directory exists

    json defaultConfig = {
        {"country", "Egypt"},
        {"city", "Cairo"},
        {"hour24", false},
        {"onAdhan", "~/.config/IslamicPrayerTimings/inAdhan.sh"}};

    std::ofstream outFile(configPath);
    if (!outFile.is_open()) {
      std::cerr << "Failed to create default config file!" << std::endl;
      return;
    }
    outFile << defaultConfig.dump(2);
    outFile.close();

    std::cout << "Created default config at: " << configPath << std::endl;
  }

  // Load config
  std::ifstream configFile(configPath);
  if (!configFile.is_open()) {
    std::cerr << "Failed to open config file!" << std::endl;
    return;
  }

  try {
    json configJson;
    configFile >> configJson;

    if (configJson.contains("country")) country = configJson["country"];
    if (configJson.contains("city")) city = configJson["city"];
    if (configJson.contains("hour24")) hour24 = configJson["hour24"];
    if (configJson.contains("onAdhan")) onAdhanScript = configJson["onAdhan"];

    // Expand ~ in onAdhan path
    if (!onAdhanScript.empty() && onAdhanScript[0] == '~') {
      onAdhanScript.replace(0, 1, getenv("HOME"));
    }

  } catch (const std::exception& e) {
    std::cerr << "Error parsing config: " << e.what() << std::endl;
  }
  configFile.close();
}
