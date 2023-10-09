# Islamic-Prayer-timings
A daemon or service for notifying about Islamic prayers timings on Linux using an API from aladhan.com.
![image](https://github.com/abdalrahmanshaban0/Islamic-Prayer-timings/assets/126330281/73fdb505-4794-45f9-93c7-0b144397cffa)


Dependancies:<br>
1- g++<br>
2- curl<br>
3- jq<br>
4- dunst (Or any notification server.)<br>

```shell
# Make them executable
chmod +x update_timings.sh
chmod +x set_next.sh
# Starting the program for current session
./update_timings.sh
```

You can auto start "update_timings.sh" on startup on you system.
Note: in every execution timings are updated, so every day when auto start it will be upated.

```shell
# Example for auto starting command:
cd $(Path to project dir) && ./update_timings.sh
```
