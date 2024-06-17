# Islamic-Prayer-timings
A daemon or service for notifying about Islamic prayers timings on Linux using an API from aladhan.com.
![image](https://github.com/abdalrahmanshaban0/Islamic-Prayer-timings/assets/126330281/73fdb505-4794-45f9-93c7-0b144397cffa)


Dependancies:<br>
1- g++<br>
2- curl<br>
3- jq<br>
4- dunst (Or any notification server.)<br>

```shell
# First, you must be in same directory as the project
# Make them executable
chmod +x update_timings.sh
chmod +x set_next.sh
# If you will use the prayer_timer.patch with slstatus
chmod +x update_rem.sh
# Starting the program for current session
./update_timings.sh
```

You can auto start "update_timings.sh" on startup on you system.
Note: You must change every absolute path used in this project to yours!!.
Note: in every execution timings are updated, so every day when auto start it will be upated.
