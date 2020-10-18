/*
  Zone.h - Library for controlling lawn watering zones.
  Created by David A. O'Neill, October 10, 2010.
  Released into the public domain.
*/
#ifndef Zone_h
#define Zone_h
#include <Arduino.h>

class Zone
{
private:
  int _pin;

  void setStartTimestamp(unsigned long startTimestamp);
  void setEndTimestamp();
  bool _isOn;

public:
  unsigned long _durationMs;
  unsigned long _endTimestamp;
  unsigned long _startTimestamp;
  Zone();
  Zone(int pin);

  int getPin();
  bool getIsOn();
  unsigned long getDurationMs();
  unsigned long getRemainingMs();

  bool isFinished();

  void setDurationMs(unsigned long durationMs);
  void turnOn();
  void turnOff();
};

#endif
