# Islamic-Prayer-timings
A daemon or service for notifying about Islamic prayers timings on Linux using an API from aladhan.com.
![image_2024-06-17_12-37-07](https://github.com/abdalrahmanshaban0/Islamic-Prayer-Timings/assets/126330281/f340727d-51eb-434e-9491-b24a56e6d326)

![1718618138](https://github.com/abdalrahmanshaban0/Islamic-Prayer-Timings/assets/126330281/3c738d60-5121-48ad-b576-4b57497ab790)


Dependancies:<br>
1- g++<br>
2- curl<br>
3- nlohmann-json<br>
4- dunst (Or any notification server.)<br>

```shell
g++ main.cpp -o IPT
```

You can auto start "IPT" on startup on you system.<br>

Note: You must change every absolute path used in this project to yours!!.<br>
Note: in every execution timings are updated, so every day when auto start it will be updated if there's internet connection.
