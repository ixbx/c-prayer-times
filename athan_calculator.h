// Resources:
// https://radhifadlillah.com/blog/2020-09-06-calculating-prayer-times/

#ifndef ATHAN_CALCULATOR_HEADER
#define ATHAN_CALCULATOR_HEADER
#define PI 3.14159265358979323846
#define PI_2 1.57079632679489661923

struct georgian_date {
  int day;
  int month;
  int year;
};

struct p_time {
  int hour;
  int minute;
  int second;
};

struct float_prayers {
  float fajr;
  float sunrise;
  float zuhr;
  float asr;
  float maghrib;
  float isha;
};

struct prayers {
  struct p_time fajr;
  struct p_time sunrise;
  struct p_time zuhr;
  struct p_time asr;
  struct p_time maghrib;
  struct p_time isha;
};

float atan_degrees(float degrees);
float acot(float x);
float tan_degrees(float degrees);
float sin_degrees(float degrees);
float cos_degrees(float degrees);

int is_date_after(struct georgian_date from, struct georgian_date to);
int is_time_after(struct p_time from, struct p_time to);
float get_julian_date(struct georgian_date, int time_zone);

float calculate_sun_declination(float julian_date);

float calculate_equation_of_time(float julian_date);

float calculate_sun_transit_time(float equation_of_time, float time_zone,
                                 float longitude);

struct float_prayers
calculate_sun_altitude_for_prayers(float fajr_angle, float isha_angle,
                                   float elevation, float asr_shadow_factor,
                                   float sun_declination, float latitude);

float calculate_hour_angle(float sun_altitude, float latitude,
                           float sun_transit_time, float sun_declination);
struct p_time calculate_real_time(float time);
struct prayers calculate_prayer_times(float time_zone, float latitude,
                                            float longitude, float elevation,
                                            struct georgian_date current_date,
                                            float asr_shadow_factor,
                                            float fajr_sun_angle, float isha_sun_angle);

#endif
