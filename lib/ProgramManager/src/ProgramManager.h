/*
  ProgramManager.h - Library for managing watering zone managers.
  Created by David A. O'Neill, February 21, 2021.
  Released into the public domain.
*/

#ifndef ProgramManager_h
#define ProgramManager_h

#include <Config.h>
#include <TimeManager.h>
#include <ZoneManager.h>

class ProgramManager
{
  ZoneManager _zoneManagers[NUM_OF_PROGRAMS];
  int _currentZoneManagerIndex = -1;
  WateringMode _mode = MODE_AUTO;

  bool shouldStartWatering();
  bool isWatering();
  int zoneManagerShouldStart();
  void updateZoneManagers();
  void resetCurrentIndex();
  void setCurrentIndex(int index);
  int getCurrentIndex();

public:
  ProgramManager();
  ProgramManager(int pins[NUM_OF_ZONES], TimeManager *timeManager);

  bool loadProgramSettings(int program, Settings settings);
  Status getProgramStatus(int program);
  void setMode(WateringMode mode);
  void setWeatherAdjustRate(float weatherAdjustRate);
  void startWatering();
  void stopWatering();
  void nextZone();
  void manage();
};

#endif
