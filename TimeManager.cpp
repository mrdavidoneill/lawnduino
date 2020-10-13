#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "TimeManager.h"

TimeManager::TimeManager(){};

TimeManager::TimeManager(NTPClient *timeClient)
{
  _timeClient = timeClient;
  syncTime();
}

void TimeManager::syncTime()
{
  DEBUG_MSG("Syncing...");
  _timeClient->update();
  _hours = _timeClient->getHours();
  _mins = _timeClient->getMinutes();

  if ((0 > _hours) || (_hours > 23))
  {
    _hours = 0;
  }

  if ((0 > _mins) || (_mins > 59))
  {
    _mins = 0;
  }

  _msSinceSync = millis();
  _msSinceTick = millis();
  DEBUG_MSG("Ok\n");
}

void TimeManager::manage()
{
  int now = millis();

  if (now > (_msSinceSync + TIME_SYNC_RATE_MS))
  {
    _msSinceSync = now;
    syncTime();
  }
  tick();
}

void TimeManager::tick()
{
  int now = millis();

  //  if (now > (_msSinceTick + MS_IN_MIN))
  if (now > (_msSinceTick + 1000))

  {
    _msSinceTick = now;
    incrementMinute();
  }
}

void TimeManager::incrementMinute()
{
  _mins++;
  if (_mins >= 60)
  {
    _mins = 0;
    incrementHour();
  }

  DEBUG_MSG("%d: %d\n", _hours, _mins);
}

void TimeManager::incrementHour()
{
  _hours++;
  if (_mins >= 24)
  {
    _hours = 0;
  }
}

bool TimeManager::isTime(int hours, int mins)
{
  return (_hours == hours) && (_mins == mins);
}
