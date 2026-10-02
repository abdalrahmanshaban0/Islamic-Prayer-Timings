# Islamic-Prayer-timings

<p align="center">
  <img width="576" height="229" alt="image" src="https://github.com/user-attachments/assets/9cea9517-6536-4273-bd88-5e10717905bb" />
</p>

<p align="center">
<img width="560" height="259" alt="image" src="https://github.com/user-attachments/assets/5e2330d9-1b8c-47f1-afbc-7641168f36f9" />
</p>

<p align="center">
<img width="558" height="551" alt="image" src="https://github.com/user-attachments/assets/0280804e-cb0b-4ee8-ae2f-65642dee40ae" />
</p>

## Install on Arch-based distributions
The project is available on the [AUR](https://aur.archlinux.org/packages/islamic-prayer-timings). To install it using AUR helper like `yay` or `paru`:
```
paru -S islamic-prayer-timings
```


## Build and run
```shell
git clone https://github.com/abdalrahmanshaban0/Islamic-Prayer-Timings/

cd Islamic-Prayer-Timings
mkdir build
cd build
cmake ..
make

# non-daemon
./islamic-prayer-timings

# daemon
./islamic-prayer-timings -d
  #OR
./islamic-prayer-timings --daemon
```

## Setting your location
Prayer timings can be fetched either by coordinates (usually more accurate) or by city and country.

### Using coordinates
Pass your latitude and longitude on the command line:
```shell
./islamic-prayer-timings --lat 30.0444 --long 31.2357

# works with the daemon too
./islamic-prayer-timings -d --lat 30.0444 --long 31.2357
```

Both `--lat` and `--long` must be given together. Latitude must be between -90 and 90, and longitude between -180 and 180.

### Using the config file
The config file is located at `~/.config/IslamicPrayerTimings/config`. It is created with default values the first time you run the program.

```json
{
  "country": "Egypt",
  "city": "Cairo",
  "latitude": 30.0444,
  "longitude": 31.2357,
  "hour24": false,
  "onAdhan": "~/.config/IslamicPrayerTimings/inAdhan.sh"
}
```

| Key | Description |
|-----|-------------|
| `country`, `city` | Location used when no coordinates are set |
| `latitude`, `longitude` | Location coordinates. Must be given together |
| `hour24` | `true` for 24-hour time, `false` for 12-hour time (AM/PM) |
| `onAdhan` | Script to run when it is time for prayer |

### Location priority
If more than one location is set, this is the order used:

1. `--lat` and `--long` on the command line
2. `latitude` and `longitude` in the config file
3. `country` and `city` in the config file

> **Note:** the default config includes coordinates, so `city` and `country` are ignored until you remove the `latitude` and `longitude` keys. To use a city instead, delete those two keys from the config.

If only one of `latitude` or `longitude` is set in the config, the program exits with an error.
