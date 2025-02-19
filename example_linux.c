#include "athan_calculator.h"
#include <stdio.h>

#include <time.h>

// Makkah location
#define LATITUDE 21.4241
#define LONGITUDE 39.8173
#define ELEVATION 277

#define TIME_ZONE 3

#define ASR_SHADOW_FACTOR 1
#define FAJR_SUN_ANGLE 18.5
#define ISHA_SUN_ANGLE 18.5

int main() {
  time_t t = time(NULL);
  struct tm lt;

  localtime_r(&t, &lt);

  struct georgian_date current_date = {lt.tm_mday, lt.tm_mon + 1,lt.tm_year + 1900};
  printf("Current date: %02d/%02d/%04d\n", current_date.day, current_date.month, current_date.year);
  printf("Current time: %02d:%02d:%02d\n\n", lt.tm_hour, lt.tm_min, lt.tm_sec);

  struct prayers prayers = calculate_prayer_times(TIME_ZONE, LATITUDE, LONGITUDE, ELEVATION, current_date, ASR_SHADOW_FACTOR, FAJR_SUN_ANGLE, ISHA_SUN_ANGLE);
  printf("%02d:%02d:%02d - Fajr\n", prayers.fajr.hour, prayers.fajr.minute, prayers.fajr.second);
  printf("%02d:%02d:%02d - Sunrise\n", prayers.sunrise.hour, prayers.sunrise.minute, prayers.sunrise.second);
  printf("%02d:%02d:%02d - Zuhr\n", prayers.zuhr.hour, prayers.zuhr.minute, prayers.zuhr.second);
  printf("%02d:%02d:%02d - Asr\n", prayers.asr.hour, prayers.asr.minute, prayers.asr.second);
  printf("%02d:%02d:%02d - Maghrib\n", prayers.maghrib.hour, prayers.maghrib.minute, prayers.maghrib.second);
  printf("%02d:%02d:%02d - Isha\n\n", prayers.isha.hour, prayers.isha.minute, prayers.isha.second);

  return 0;
}
