# Islamic-Prayer-timings
![Screenshot-2025-06-28_09:19:19](https://github.com/user-attachments/assets/fd61a701-1b34-4adf-a271-641c56d67dfb)
## Requirements & Features
- Find an API to get the correct timings for your country and city.
- Read values of **country**,  **city**, **hour24** and **onAdhan** from `~/.config/IslamicPrayerTimings`. This is configuration file example:
```json
{
"country": "Egypt",
"city": "Cairo",
"hour24": false,
"onAdhan": "~/.config/IslamicPrayerTimings/inAdhan.sh"
}
```
- Get and store the timings and hijri date.
- Make a cache contains the latest response in case starting the daemon without internet.
- Make a daemon that calculates the next pray timing, time difference and starts a timer with the countdown.
<div align="center">
  <img src="https://github.com/user-attachments/assets/0320f1a4-1d5d-4038-9bad-d28bdded9a8f" alt="Screenshot" width="800">
</div>

- When the timer is finished send a notification and a user shell script is run (optional) e.g. lock the screen, play a sound, .... This is example of such a script:
```shell
#!/bin/bash
# play adhan audio
AUDIO_FILE="/home/abdalrahman/adhan_mansour_al_zahrani.mp3"
mpv --no-video --loop=no --really-quiet "$AUDIO_FILE" &
sleep 10
# lock screen (for hyprland)
hyprlock
```
## Development
- Best practice is to use bash script, but I'll use C++ as my knowledge of bash is very basic yet.
- Dependencies:
	- **g++** : C++ compiler
	- **CMake** : building tool for C++
	-  **nlohmann-json** : to parse json files for read
	- **curl** : to send http get request to retrieve the timings from the API
	- **libnotify** : for sending notification to a notification daemon
	- **mako** (Any suitable notification server) : notification daemon to receive and show notification
- The API I'll retrieve the timings from is :
`https://api.aladhan.com/v1/timingsByCity?city=Cairo&country=Egypt`
test the response using **postman**
- Response is JSON, I need to send a GET request with `curl` to retrieve that response. 
- AI tools like ChatGPT will be very useful to give examples and explains how to use curl.h and json.hpp, generally saving time of reading documentation.
- Rest of the logic is simple
## Build and run
```shell
git clone https://github.com/abdalrahmanshaban0/Islamic-Prayer-Timings/

cd Islamic-Prayer-Timings
mkdir build
cd build
cmake ..
make

./Islamic-Prayer-Timings
```
## Waybar custom module
You can see the [wiki](https://github.com/Alexays/Waybar/wiki/Module:-Custom) to know to add a custom module to Waybar.
I want to show the next prayer timing name and a countdown timer. With tooltip on hover with the day prayer timings.
```json
  "custom/prayerTimings": {
    "exec": "/home/abdalrahman/CLionProjects/IslamicPrayerTimings/build/Islamic_Prayer_Timings",
    "format": "{text} 🕋",
    "return-type": "json",
    "tooltip": true
  }
```
