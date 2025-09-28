# Islamic-Prayer-timings
See my post about this project in my technical blog [here](https://sudostart.com/islamic-prayer-timings-for-linux/)

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
