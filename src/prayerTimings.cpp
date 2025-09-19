#include <curl/curl.h>

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "../include/prayer_timings.h"
#include "../include/utils.h"
#include "../include/waybar.h"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

// config variables
static string country, city;
static bool hour24;
extern string onAdhanScript;

extern vector<string> prayerTimings;
extern string waybarTooltip;

extern mutex dataMutex;
extern condition_variable timingsReady;

int loadConfig() {
  const char* homeEnv = std::getenv("HOME");
  if (!homeEnv) {
    std::cerr << "HOME environment variable not set!\n";
    return 1;
  }

  const std::string configDir =
      std::string(homeEnv) + "/.config/IslamicPrayerTimings";
  const std::string configPath = configDir + "/config";

  try {
    // If config does not exist, create it with default values
    if (!fs::exists(configPath)) {
      fs::create_directories(configDir);  // ensure directory exists

      nlohmann::json defaultConfig = {
          {"country", "Egypt"},
          {"city", "Cairo"},
          {"hour24", false},
          {"onAdhan", "~/.config/IslamicPrayerTimings/inAdhan.sh"}};

      std::ofstream outFile(configPath);
      if (!outFile.is_open()) {
        std::cerr << "Failed to create default config file!\n";
        return 2;
      }
      outFile << defaultConfig.dump(2);
      outFile.close();

      std::cout << "Created default config at: " << configPath << '\n';
    }
  } catch (const std::exception& e) {
    std::cerr << "Failed to prepare config: " << e.what() << '\n';
    return 3;
  }

  // Load config
  std::ifstream configFile(configPath);
  if (!configFile.is_open()) {
    std::cerr << "Failed to open config file!\n";
    return 4;
  }

  try {
    nlohmann::json configJson;
    configFile >> configJson;

    if (configJson.contains("country"))
      country = configJson["country"].get<std::string>();
    if (configJson.contains("city"))
      city = configJson["city"].get<std::string>();
    if (configJson.contains("hour24"))
      hour24 = configJson["hour24"].get<bool>();
    if (configJson.contains("onAdhan"))
      onAdhanScript = configJson["onAdhan"].get<std::string>();

    // Expand ~ in onAdhan path
    if (!onAdhanScript.empty() && onAdhanScript[0] == '~') {
      onAdhanScript.replace(0, 1, homeEnv);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error parsing config: " << e.what() << '\n';
    return 5;
  }

  return 0;
}

int loadCachedTimings() {
  const char* homeEnv = std::getenv("HOME");
  if (!homeEnv) {
    std::cerr << "HOME environment variable not set.\n";
    return 1;
  }

  fs::path filePath =
      fs::path(homeEnv) / ".local/share/IslamicPrayerTimings/timings.json";

  if (!fs::exists(filePath)) {
    return 2;
  }

  json j;
  try {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
      std::cerr << "Cannot open cache file: " << filePath << '\n';
      return 3;
    }
    inFile >> j;
  } catch (const std::exception& e) {
    std::cerr << "Error reading JSON: " << e.what() << '\n';
    return 4;
  }

  try {
    const auto& t = j.at("data").at("timings");
    prayerTimings = {
        t.at("Fajr").get<std::string>(),    t.at("Sunrise").get<std::string>(),
        t.at("Dhuhr").get<std::string>(),   t.at("Asr").get<std::string>(),
        t.at("Maghrib").get<std::string>(), t.at("Isha").get<std::string>()};
  } catch (const std::exception& e) {
    std::cerr << "Invalid JSON structure: " << e.what() << '\n';
    return 5;
  }

  return 0;
}

static size_t WriteCallback(void* contents, const size_t size,
                            const size_t nmemb, string* output) {
  const size_t totalSize = size * nmemb;
  output->append(static_cast<char*>(contents), totalSize);
  return totalSize;
}

int getTimings(std::vector<std::string>& timings, std::string& hdate) {
  std::string readBuffer;

  // Initialize CURL
  CURL* curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize CURL\n";
    return 1;
  }

  const std::string url =
      "https://api.aladhan.com/v1/timingsByCity?city=" + city +
      "&country=" + country;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
    std::cerr << "Request failed: " << curl_easy_strerror(res) << '\n';
    curl_easy_cleanup(curl);
    return 1;
  }

  curl_easy_cleanup(curl);

  try {
    const json j = json::parse(readBuffer);

    const char* home = std::getenv("HOME");
    if (!home) {
      std::cerr << "HOME environment variable not set.\n";
      return 3;
    }

    const std::string dirPath =
        std::string(home) + "/.local/share/IslamicPrayerTimings";
    const std::string filePath = dirPath + "/timings.json";

    std::filesystem::create_directories(dirPath);
    std::ofstream(filePath) << j.dump(2);

    const auto& t = j["data"]["timings"];
    timings = {t["Fajr"].get<std::string>(),    t["Sunrise"].get<std::string>(),
               t["Dhuhr"].get<std::string>(),   t["Asr"].get<std::string>(),
               t["Maghrib"].get<std::string>(), t["Isha"].get<std::string>()};

    const auto& hijri = j["data"]["date"]["hijri"];
    hdate = hijri["day"].get<std::string>() + " " +
            hijri["month"]["en"].get<std::string>() + " " +
            hijri["year"].get<std::string>();
  } catch (const std::exception& e) {
    std::cerr << "JSON parse error: " << e.what() << '\n';
    return 2;
  }

  return 0;
}

void updateWorker() {
  while (true) {
    if (!isInternetAvailable()) {
      waitInternet(900);
      continue;
    }

    std::vector<std::string> newTimings;
    std::string newHijri;

    const int result = getTimings(newTimings, newHijri);
    if (result == 0) {
      {
        std::lock_guard lock(dataMutex);
        prayerTimings = std::move(newTimings);
        const string hijriDate = std::move(newHijri);
        waybarTooltip = formatTooltip(prayerTimings, hijriDate, names, hour24);
      }
      timingsReady.notify_one();
    } else {
      std::cerr << "Failed to update timings (error code " << result
                << "). Retrying in 10 minutes.\n";
      std::this_thread::sleep_for(std::chrono::minutes(10));
      continue;
    }
    const int sleepDuration = secondsUntilMidnight();
    std::this_thread::sleep_for(std::chrono::seconds(sleepDuration + 1));
  }
}
