/*
  ZoneManager.h - Library for managing watering zones.
  Created by David A. O'Neill, October 10, 2010.
  Released into the public domain.
*/

#ifndef ZoneManager_h
#define ZoneManager_h

#include "Constants.h"

#include "Zone.h"

class ZoneManager
{
  Zone _zones[NUM_OF_ZONES];
  int _currentZoneIndex = -1;
  int _startTime[NUM_OF_HH_MIN];
  bool _wateringDays[NUM_OF_WEEKDAYS];

  void updateZones();
  bool isTasks();

public:
  ZoneManager();
  ZoneManager(int pins[NUM_OF_ZONES]);

  void manage();
  void startWatering();
  void stopWatering();
  void nextZone();
  bool loadSettings(Settings settings);
  void setZoneDuration(int zoneIndex, unsigned long durationMs);
};

#endif
