/*
  RainAdjust.h - Library for calculating rain adjust percentage.
  Created by David A. O'Neill, October 21, 2020.
  Released into the public domain.
*/
#ifndef RainAdjust_h
#define RainAdjust_h

#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "Secrets.h"
#include <ZoneManager.h>

#define DEFAULT_TARGET 50

#define TIMEOUT (5 * 60 * 1000)
#define UPDATE_RATE (60 * 60 * 1000)

#define HOURLY_LOG_LENGTH 3
#define WEEK_LOG_LENGTH (24 / 3 * 7)

#define MAX_SUNLIGHT_SEC 50000
#define MIN_SUNLIGHT_SEC 30000

class RainAdjust
{
public:
    RainAdjust();
    RainAdjust(WiFiClient *client, HTTPClient *http, ZoneManager *zoneManager);
    RainAdjust(float hourlyLog[HOURLY_LOG_LENGTH], float weekLog[WEEK_LOG_LENGTH]);
    float getRainAdjust();
    float getSunlightAdjust();
    void manage();
    float sumHourlyLog();
    float sumWeekLog();

private:
    WiFiClient *_client;
    HTTPClient *_http;
    ZoneManager *_zoneManager;

    float _hourlyLog[HOURLY_LOG_LENGTH];
    float _weekLog[WEEK_LOG_LENGTH];
    unsigned int _targetLevel = DEFAULT_TARGET;
    unsigned long _lastLog = 0;
    unsigned long _timeOut = 0;
    unsigned long _updateRate = UPDATE_RATE;
    long _daylightSec = -1;

    void resetHourlyLog();
    void resetWeekLog();
    void addToHourlyLog(int mm);
    void addToWeekLog(unsigned int mm);
    void shiftWeekLog();

    bool fetchWeather();
    void parseWeather();
    bool timeToFetch();

    void logRainFall();
    void updateZoneManager();
    void setDaylightSec(long daylightSec);
};

#endif
