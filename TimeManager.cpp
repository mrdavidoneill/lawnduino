#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "TimeManager.h"

TimeManager::TimeManager(){};

TimeManager::TimeManager(NTPClient *timeClient)
{
  _hours = 0;
  _mins = 0;
  _day = 0;
  _timeClient = timeClient;
  _isOnline = true;
  syncTime();
}

void TimeManager::getTimeFromMillis()
{
  _hours = _timeClient->getHours();
  _mins = _timeClient->getMinutes();
  _day = _timeClient->getDay();
}

void TimeManager::syncTime()
{
  DEBUG_MSG("Syncing...");
  _timeClient->update();
  int serverHours = _timeClient->getHours();
  int serverMins = _timeClient->getMinutes();

  if (isTimeValid(serverHours, serverMins))
  {
    _hours = serverHours;
    _mins = serverMins;
    DEBUG_MSG("Ok\n");
  }
  else
  {
    DEBUG_MSG("Can't sync\n");
  }

  _msSinceSync = millis();
  _msSinceTick = millis();
}

void TimeManager::manage()
{
  if (getIsOnline())
  {
    int now = millis();
    if (isTimeToSync(now))
    {
      _msSinceSync = now;
      syncTime();
    }
  }
  tick();
}

void TimeManager::setIsOnline(bool isOnline)
{
  _isOnline = isOnline;
}

bool TimeManager::getIsOnline()
{
  return _isOnline;
}

bool TimeManager::isTimeValid(int hours, int mins)
{
  if ((0 > hours) || (hours > 23))
  {
    DEBUG_MSG("Error: hours fetched: %d\n", hours);
    return false;
  }

  if ((0 > mins) || (mins > 59))
  {
    DEBUG_MSG("Error: mins fetched: %d\n", mins);
    return false;
  }
  return true;
}

void TimeManager::tick()
{
  int now = millis();
  if (isTimeToIncrement(now))
  {
    _msSinceTick = now;
    getTimeFromMillis();
    DEBUG_MSG("%d: %d\n", _hours, _mins);
  }
}

bool TimeManager::isTimeToSync(int now)
{
  if (now > (_msSinceSync + TIME_SYNC_RATE_MS))
  {
    return true;
  }
  return false;
}

bool TimeManager::isTimeToIncrement(int now)
{
  if (now > (_msSinceTick + MS_IN_SEC))
  {
    return true;
  }
  return false;
}

bool TimeManager::isTime(int hours, int mins)
{
  return (_hours == hours) && (_mins == mins);
}

int TimeManager::getDaysSinceSunday()
{
  return _day;
}
