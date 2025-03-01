#include "athan_calculator.h"
#include <math.h>

float sin_degrees(float degrees) { return sin(degrees * (PI / 180.0)); }

float cos_degrees(float degrees) { return cos(degrees * (PI / 180.0)); }

float atan_degrees(float degrees) { return atan(degrees * (PI / 180.0)); }

float tan_degrees(float degrees) { return tan(degrees * (PI / 180.0)); }

float acot(float x) {
  double result;
  if (x > 0)
    result = atan(1.0 / x);
  else if (x < 0)
    result = atan(1.0 / x) + PI;
  else
    result = PI_2;

  return result * (180.0 / PI);
}

/*
 * Calculates the latitude at which the sun is directly overhead.
 *
 * Arguments:
 *   float julian_date: Julian date format of provided date
 * Returns:
 *   float: Sun declination value
 */
float calculate_sun_declination(float julian_date) {
  float t = 2 * PI * (julian_date - 2451545) / 365.25;
  return 0.37877 + 23.264 * sin_degrees(57.297 * t - 79.547) +
         0.3812 * sin_degrees(2 * 57.297 * t - 82.682) +
         0.17132 * sin_degrees(3 * 57.297 * t - 59.722);
}

/*
 * Returns true if the first date is after the second date
 *
 * Arguments:
 *   georgian_date date     : The date to compare
 *   georgian_date to_check : The date to check if after or before
 * Returns:
 *   int: 1 if the date is after the date to check, 0 if not.
 */
int is_date_after(struct georgian_date date, struct georgian_date to_check) {
  if (date.year > to_check.year)
    return 1;
  else {
    if (date.month > to_check.month)
      return 1;
    else {
      if (date.day > to_check.day)
        return 1;
    }
  }

  return 0;
}

/*
 * Returns true if the first time is after the second time
 *
 * Arguments:
 *   p_time from : The time to compare
 *   p_time to   : The time to check if after or before
 * Returns:
 *   int: 1 if the time is after the time to check, 0 if not.
 */
int is_time_after(struct p_time from, struct p_time to) {
  if (from.hour > to.hour)
    return 1;
  if (from.hour < to.hour)
    return 0;
  if (from.minute > to.minute)
    return 1;
  if (from.minute < to.minute)
    return 0;
  return from.second > to.second;
}

/*
 * Converts a Georgian Date into a Julian Date
 * https://aa.usno.navy.mil/faq/JD_formula
 *
 * Arguments:
 *   georgian_date date : The date to convert
 *   int time_zone      : Time zone of current location
 * Returns:
 *   float: The Julian Date
 */
float get_julian_date(struct georgian_date date, int time_zone) {
  int Y = date.year;
  int M = date.month;
  int D = date.day;
  int H = 12;
  int m = 0;
  int s = 0;
  int Z = time_zone;

  if (M <= 2) {
    M += 12;
    Y--;
  }

  int B = 0;
  struct georgian_date special_date = {14, 10, 1582};
  if (is_date_after(date, special_date)) {
    int A = Y / 100;
    B = 2 + (A / 4) - A;
  }

  // Integer division is intentional.
  return 1720994.5 + (int)(365.25 * Y) + (int)(30.6001 * (M + 1)) + B + D +
         ((H * 3600 + m * 60 + s) / 86400) - (Z / 24);
}

/*
 * Calculates the curve that represents the angular offset of the Sun from its
 * mean position on the celestial sphered as viewed from Earth.
 *
 * Arguments:
 *   float julian_date : The Julian Date format of the current date
 * Returns:
 *   float: The equation of time
 */
float calculate_equation_of_time(float julian_date) {
  float u = (julian_date - 2451545) / 36525;
  float l0 = 280.46607 + 36000.7698 * u;
  float et1000 =
      -(1789 + 237 * u) * sin_degrees(l0) - (7146 - 62 * u) * cos_degrees(l0) +
      (9934 - 14 * u) * sin_degrees(2 * l0) -
      (29 + 5 * u) * cos_degrees(2 * l0) + (74 + 10 * u) * sin_degrees(3 * l0) +
      (320 - 4 * u) * cos_degrees(3 * l0) - 212 * sin_degrees(4 * l0);

  return et1000 / 1000;
}

/*
 * Calculates the daily moment when the Sun culminates on the observer's
 * meridian.
 *
 * Arguments:
 *   float equation_of_time: Equation of time
 *   float time_zone       : Observer's time zone
 *   float longitude       : Observer's longitude
 * Returns:
 *   float: Sun transit time
 */
float calculate_sun_transit_time(float equation_of_time, float time_zone,
                                 float longitude) {
  return 12 + time_zone - (longitude / 15) - (equation_of_time / 60);
}

/*
 * Calculates the sun altitudes for each prayer except zuhr.
 *
 * Arguments:
 *   float fajr_angle : Sun's angle at fajr time
 *   float isha_angle : Sun's angle at isha time
 *   float elevation  : Observer's elevation
 *   float asr_shadow_factor : The length of the shadow of an object at asr time
 *   float latitude   : Observer's latutude
 * Returns:
 *   float_prayers: Sun altitudes for each prayer except zuhr;
 */
struct float_prayers
calculate_sun_altitude_for_prayers(float fajr_angle, float isha_angle,
                                   float elevation, float asr_shadow_factor,
                                   float sun_declination, float latitude) {
  struct float_prayers sa; // sun altitude

  sa.fajr = -(fajr_angle);
  sa.sunrise = -0.8333 - (0.0347 * sqrtf(elevation));
  sa.asr =
      acot(asr_shadow_factor + tan_degrees(fabs(sun_declination - latitude)));
  sa.maghrib = sa.sunrise;
  sa.isha = -(isha_angle);

  return sa;
}

/*
 * Calculates the dihedral angle between the meridian plane and the hour circle.
 * https://en.wikipedia.org/wiki/Hour_angle
 *
 * Arguments:
 *   float sun_altitude    : Sun's altitude
 *   float latitude        : Observer's latitude
 *   float sun_transit_time: Sun transit time
 *   float sun_declination : Sun declination
 */
float calculate_hour_angle(float sun_altitude, float latitude,
                           float sun_transit_time, float sun_declination) {
  double cos_ha = (sin_degrees(sun_altitude) -
                   sin_degrees(latitude) * sin_degrees(sun_declination)) /
                  (cos_degrees(latitude) * cos_degrees(sun_declination));
  return acos(cos_ha) * (180.0 / PI);
}

struct p_time calculate_real_time(float time) {
  struct p_time rt;

  rt.hour = (int)time;
  float min = 60.0 * (time - rt.hour);
  rt.minute = (int)min;
  rt.second = 60.0 * (min - rt.minute);

  return rt;
}

/*
 * Calculates the prayer times for a given date.
 *
 * Arguments:
 *   float time_zone : Observer's time zone
 *   float latitude  : Observer's latitude
 *   float longitude : Observer's longitude
 *   georgian_date current_date : The date to calculate prayer times for
 *   float asr_shadow_factor    : The length of shadows at asr time
 *   float fajr_sun_angle       : The sun angle at fajr time
 * Returns:
 *   prayers: The time of each prayer in 24 hour format.
 */
struct prayers calculate_prayer_times(float time_zone, float latitude,
                                      float longitude, float elevation,
                                      struct georgian_date current_date,
                                      float asr_shadow_factor,
                                      float fajr_sun_angle,
                                      float isha_sun_angle) {
  float jd = get_julian_date(current_date, time_zone);
  float sun_declination = calculate_sun_declination(jd);
  float eq = calculate_equation_of_time(jd);
  float sun_transit_time = calculate_sun_transit_time(eq, time_zone, longitude);
  struct float_prayers sun_altitudes = calculate_sun_altitude_for_prayers(
      fajr_sun_angle, isha_sun_angle, elevation, asr_shadow_factor,
      sun_declination, latitude);

  struct float_prayers ha; // hour angles

  ha.fajr = calculate_hour_angle(sun_altitudes.fajr, latitude, sun_transit_time,
                                 sun_declination);
  ha.sunrise = calculate_hour_angle(sun_altitudes.sunrise, latitude,
                                    sun_transit_time, sun_declination);
  ha.asr = calculate_hour_angle(sun_altitudes.asr, latitude, sun_transit_time,
                                sun_declination);
  ha.maghrib = calculate_hour_angle(sun_altitudes.maghrib, latitude,
                                    sun_transit_time, sun_declination);
  ha.isha = calculate_hour_angle(sun_altitudes.isha, latitude, sun_transit_time,
                                 sun_declination);

  struct float_prayers fp;

  fp.fajr = sun_transit_time - ha.fajr / 15;
  fp.sunrise = sun_transit_time - ha.sunrise / 15;
  fp.zuhr = sun_transit_time + (2.0 / 6.0);
  fp.asr = sun_transit_time + ha.asr / 15;
  fp.maghrib = sun_transit_time + ha.maghrib / 15;
  fp.isha = sun_transit_time + ha.isha / 15;

  struct prayers p;

  p.fajr = calculate_real_time(fp.fajr);
  p.sunrise = calculate_real_time(fp.sunrise);
  p.zuhr = calculate_real_time(fp.zuhr);
  p.asr = calculate_real_time(fp.asr);
  p.maghrib = calculate_real_time(fp.maghrib);
  p.isha = calculate_real_time(fp.isha);

  return p;
}
