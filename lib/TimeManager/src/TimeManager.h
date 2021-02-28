/*
  TimeManager.h - Library for managing time.
  Created by David A. O'Neill, October 13, 2020.
  Released into the public domain.
*/

#ifndef TimeManager_h
#define TimeManager_h

#include <NTPClient.h>
#include <Config.h>

class TimeManager
{
  NTPClient *_timeClient;
  int _msSinceSync;
  int _msSinceTick;
  int _hours;
  int _mins;
  int _day;
  bool _isOnline;

  bool getIsOnline();
  void syncTime();
  void tick();
  void getTimeFromMillis();
  bool isTimeValid(int hours, int mins);
  bool isTimeToSync(int now);
  bool isTimeToIncrement(int now);
  bool isInterval(int mins);
  int getTimeInMinutes();

public:
  TimeManager();
  TimeManager(NTPClient *timeClient);
  bool isTime(int hours, int mins);
  void manage();
  int getDaysSinceSunday();
  void setIsOnline(bool isOnline);
};

#endif