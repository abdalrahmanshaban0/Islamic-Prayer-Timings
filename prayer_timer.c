#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../slstatus.h"
#include "../util.h"

void SecondsToString(int seconds, char *buffer, int buffer_size) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    snprintf(buffer, buffer_size, "%02d:%02d:%02d", hours, minutes, secs);
}

const char *
prayer_timer(const char* path){
    FILE* f = fopen(path, "r");
    char* arr[6] = {"Fajr ", "Sunrise ", "Dhuhr ", "Asr ", "Maghrib ", "Isha "};
    int idx = 0, rem = 0;
    fscanf(f, "%d", &idx);
    fscanf(f, "%d", &rem);
    fclose(f);
    char tmr[30];
    strcpy(tmr, arr[idx]);
    char temp[10];
    SecondsToString(rem, temp, 10);
    strcat(tmr, temp);
    return bprintf(tmr);
}
