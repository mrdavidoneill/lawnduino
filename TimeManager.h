/*
  TimeManager.h - Library for managing time.
  Created by David A. O'Neill, October 13, 2010.
  Released into the public domain.
*/

#ifndef TimeManager_h
#define TimeManager_h

#include <NTPClient.h>
#include "Constants.h"

class TimeManager
{
    NTPClient *_timeClient;
    int _msSinceSync;
    int _msSinceTick;
    int _hours;
    int _mins;

    void syncTime();
    void tick();
    void incrementMinute();
    void incrementHour();
    bool isTime(int hours, int mins);

public:
    TimeManager();
    TimeManager(NTPClient *timeClient);
    void manage();
};

#endif