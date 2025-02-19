# C Prayer Times

<h3 align="center">Islamic prayer time calculator source for any projects written in the C programming language.</h3>
<h3 align="center">مصدر حاسبة وقت الصلاة الإسلامية لأي مشاريع مكتوبة بلغة البرمجة C.</h3>
<p align="center">
<img src="https://static.vecteezy.com/system/resources/previews/040/564/502/non_2x/islamic-frame-background-mosque-and-lantern-illustration-border-frame-for-ramadan-design-eid-and-islamic-festivals-vector.jpg" style="width:50%">
</p>

<p align="center">Based on: https://radhifadlillah.com/blog/2020-09-06-calculating-prayer-times/</p>

<br />
<br />
<br />

## How to use

You can use this source for both Windows and Linux, MacOS is untested. To use it, you need to copy the files `athan_calculator.c` and `athan_calculator.h` to your project. In your project, you will need to get your current **georgian** date, there is a structure built into `athan_calculator.h` that the function `calculate_prayer_times` utilizes.

```c
struct georgian_date {
  int day;
  int month;
  int year;
};
```

There are multiple parameters you need to account for in the calculation:
1. Latitude
2. Longitude
3. Elevation
4. Time zone
5. Asr shadow factor
6. Fajr sun angle
7. Isha sun angle

### Parameter explanations

**Longitude and Latitude**

This should be the approximate location of where you live. If you want, you can set it to exactly where you live, but for security reasons you should only set it to the city you live in.

<br>

**Time zone**

This should be the time zone of the city you live in in GMT (Greenwich Mean Time) format. For example, if you live in Saudi Arabia, your time zone is GMT+3, which means the value this parameter must be set to is 3.

<br>

**Asr shadow factor**

The shadow factor for Asr is the length of the shadow relative to an object. If you put a stick in the ground with the length of the stick from the ground is 10cm, and the length of the shadow casted by the sun of the stick is also 10cm, the shadow factor is 1, if the shadow casted by the sun from the sick is 20cm, the shadow factor is 2.

There are multiple different preferences for this, see the table below for which one you need:
| Jurist  | Shadow Factor |
| ------------- | ------------- |
| Shafi'i, Maliki, Ja'fari, Hanbali  | 1  |
| Hanafi | 2  |

SOURCE: http://praytimes.org/calculation#Asr

<br>

**Fajr & Isha sun angle**

Fajr starts when the sun's light is seen after it was completely invisible, this is called [astronomical dawn](https://www.timeanddate.com/astronomy/astronomical-twilight.html), while Isha starts when the light of the sun is completely gone.

For this, you can refer to the table below to see which organization suits you
| Organization  | Angle of the sun (Fajr) | Angle of the sun (Isha) | Region |
| ------------- | ----------------------- | ----------------------- | ------ |
| Muslim World League | 18 degrees | 17 degrees | Europe, The Far East, Parts of the USA |
| Egyptian General Authority of Survey | 19.5 degrees | 17.5 degrees | Africa, Syria, Iraq, Lebanon, Malaysia, Parts of the USA |
| University Of Islamic Sciences, Karachi | 18 degrees | 18 degrees | Pakistan, Bangladesh, India, Afghanistan, Parts of Europe |
| Umm Al-Qura | 18.5 Degrees (19 degrees before 1430 hijri) | 90 minutes after the Sunset Prayer. 120 minutes (in Ramadan only) |	The Arabian Peninsula |
| Islamic Society of North America | 15 degrees | 15 degrees | Parts of the USA, Canada, Parts of the UK |
| Union des Organisations Islamiques de France | 12 degrees | 12 degrees | France region |
| Majlis Ugama Islam Singapura | 20 degrees | 18 degrees | Singapore region |

SOURCE: https://fiqhcouncil.org/the-suggested-calculation-method-for-fajr-and-isha/

<br>

These parameters can be used accordingly
```c
struct prayers prayers = calculate_prayer_times(TIME_ZONE, LATITUDE, LONGITUDE, ELEVATION, current_date, ASR_SHADOW_FACTOR, FAJR_SUN_ANGLE, ISHA_SUN_ANGLE);
```

### Windows Example

To implement this in your windows Visual Studio project, you must first copy `athan_calculator.h` and `athan_calculator.c` to your project. Once this is done, you now need to calculate your current georgian date and put it in the `georgian_date` structure for use by the `calculate_prayer_times` function:

![image](https://github.com/user-attachments/assets/1fb4b0a2-d935-497a-81b9-f2b84215a8aa)


```c
#include <time>

int main() {
  time_t t = time(NULL);
  struct tm lt;

  localtime_s(&lt, &t);

  struct georgian_date current_date = { lt.tm_mday, lt_tm_mon + 1, lt.tm_year + 19000 };

  struct prayers prayers = calculate_prayer_times(3, 21.4241, 39.8173, 277, current_date, 1, 18.5, 18.5);
}
```

Then you can query a prayer time like so:
```c
prayers.fajr.hour
prayers.fajr.minute
prayers.fajr.second
```

You can do the same with `sunrise`, `zuhr`, `asr`, `maghrib`, and `isha`.

![image](https://github.com/user-attachments/assets/2bc9a5cb-977f-4b49-a0b2-1328039f080a)


### Linux Example

The Linux implementation is the same as Windows, except instead of using `localtime_s`, you must use `localtime_r`.

```c
#include <time>

int main() {
  time_t t = time(NULL);
  struct tm lt;

  localtime_r(&t, &lt);

  struct georgian_date current_date = { lt.tm_mday, lt_tm_mon + 1, lt.tm_year + 19000 };

  struct prayers prayers = calculate_prayer_times(3, 21.4241, 39.8173, 277, current_date, 1, 18.5, 18.5);
}
```

You can test it with Makkah parameters by cloning this repository and running `run_linux.sh`:

```c
git clone https://github.com/exploitfreaker/c-prayer-times.git
cd c-prayer-times
./run_linux.sh
```

![image](https://github.com/user-attachments/assets/6432748a-3b14-4556-9412-9bfd0c3a0e9a)



## Example usage

**Calculator being used for identifying the prayer times in a linux terminal**


![image](https://github.com/user-attachments/assets/51fb149e-99c9-4d5e-8c12-fffd4b04f3cd)



**Calculator being used as an extension for [slstatus](https://tools.suckless.org/slstatus/) on [DWM](https://dwm.suckless.org/) (Dynamic Window Manager)**


![image](https://github.com/user-attachments/assets/6215a74f-28a7-4ac0-b52e-999b8b58d455)


<br />
<br />
<br />
<br />
<br />
<br />
<br />
<br />
<br />
