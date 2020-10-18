/*
  ZoneManager.h - Library for managing watering zones.
  Created by David A. O'Neill, October 10, 2010.
  Released into the public domain.
*/

#ifndef ZoneManager_h
#define ZoneManager_h

#include "Constants.h"
#include "TimeManager.h"
#include "Zone.h"

class ZoneManager
{
  Zone _zones[NUM_OF_ZONES];
  TimeManager *_timeManager;
  int _currentZoneIndex = -1;
  int _startTime[NUM_OF_HH_MIN] = {11, 49};
  bool _wateringDays[NUM_OF_WEEKDAYS] = {1, 1, 1, 1, 1, 1, 1};
  WateringMode _mode = MODE_AUTO;

  void updateZones();
  bool isWatering();
  void autoMode();
  void setZoneDuration(int zoneIndex, unsigned long durationMs);
  unsigned long getZoneDurationMs(int zoneIndex);
  unsigned long getZoneMsRemaining(int zoneIndex);

public:
  ZoneManager();
  ZoneManager(int pins[NUM_OF_ZONES], TimeManager *timeManager);

  void manage();
  void setMode(WateringMode mode);
  void startWatering();
  void stopWatering();
  void nextZone();
  bool loadSettings(Settings settings);
  Status getStatus();
};

#endif
