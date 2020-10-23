#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "RainAdjust.h"

RainAdjust::RainAdjust() {}

RainAdjust::RainAdjust(WiFiClient *client, HTTPClient *http, ZoneManager *zoneManager)
{
    _client = client;
    _http = http;
    _zoneManager = zoneManager;
    DEBUG_MSG("Creating RainAdjust\n");
    resetHourlyLog();
    resetWeekLog();
}

RainAdjust::RainAdjust(float hourlyLog[HOURLY_LOG_LENGTH], float weekLog[WEEK_LOG_LENGTH])
{
    for (int i = 0; i < HOURLY_LOG_LENGTH; i++)
    {
        _hourlyLog[i] = hourlyLog[i];
    }

    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        _weekLog[i] = weekLog[i];
    }
}

void RainAdjust::resetHourlyLog()
{
    DEBUG_MSG("Resetting hourly log...");
    for (int i = 0; i < HOURLY_LOG_LENGTH; i++)
    {
        _hourlyLog[i] = -1;
    }
    DEBUG_MSG("Ok\n");
}

void RainAdjust::resetWeekLog()
{
    DEBUG_MSG("Resetting week log...");
    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        _weekLog[i] = 0;
    }
    DEBUG_MSG("Ok\n");
}

void RainAdjust::addToHourlyLog(int mm)
{
    bool hasLogged = false;
    int i = 0;

    while (!hasLogged && i < HOURLY_LOG_LENGTH)
    {
        if (_hourlyLog[i] == -1)
        {
            _hourlyLog[i] = mm;
            hasLogged = true;
        }
        i++;
    }
    if (!hasLogged)
    {
        resetHourlyLog();
        addToWeekLog(sumHourlyLog());
        _hourlyLog[0] = mm;
    }
}

float RainAdjust::sumHourlyLog()
{
    float sum = 0;
    for (int i = 0; i < HOURLY_LOG_LENGTH; i++)
    {
        sum += _hourlyLog[i];
    }
    return sum;
}

float RainAdjust::sumWeekLog()
{
    float sum = 0;
    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        sum += _weekLog[i];
    }
    return sum;
}

void RainAdjust::addToWeekLog(unsigned int mm)
{
    shiftWeekLog();
    _weekLog[WEEK_LOG_LENGTH - 1] = mm;
}

void RainAdjust::shiftWeekLog()
{
    for (int i = 0; i < WEEK_LOG_LENGTH - 1; i++)
    {
        _weekLog[i] = _weekLog[i + 1];
    }
}

void RainAdjust::logRainFall()
{
    DEBUG_MSG("Logging rainFall\n");
    _lastLog = millis();
    if (fetchWeather())
    {
        _timeOut = 0;
        DEBUG_MSG("Time out deactivated\n");
    }
    else
    {
        DEBUG_MSG("Time out activated\n");
        _timeOut = TIMEOUT;
    }
}

void RainAdjust::manage()
{
    if (timeToFetch())
    {
        logRainFall();
    }
}

bool RainAdjust::timeToFetch()
{
    if (_lastLog == 0)
    {
        DEBUG_MSG("Time to start log...\n");
        return true;
    }

    if (millis() > (_lastLog + _timeOut + _updateRate))
    {
        DEBUG_MSG("Time to log...\n");
        return true;
    }

    return false;
}

bool RainAdjust::fetchWeather()
{
    DEBUG_MSG("Fetching weather\n");
    DEBUG_MSG("Requesting weather...");

    if (_http->begin(API_CALL))
    {
        _http->GET();
        DEBUG_MSG("Ok\n");
        parseWeather();
        _http->end();
        return true;
    }
    return false;
}

void RainAdjust::parseWeather()
{
    DEBUG_MSG("Waiting for response...\n");
    const size_t capacity = JSON_ARRAY_SIZE(1) + 2 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(14) + 310;
    DynamicJsonDocument doc(capacity * 2);
    deserializeJson(doc, _http->getStream());

    float main_temp = doc["main"]["temp"].as<float>();                     // 13.89
    float rain_1h = doc["rain"]["1h"].as<float>();                         // 1.85
    unsigned long sys_sunrise = doc["sys"]["sunrise"].as<unsigned long>(); // 1603262174
    unsigned long sys_sunset = doc["sys"]["sunset"].as<unsigned long>();   // 1603301463
    long daylightSec = sys_sunset - sys_sunrise;
    setDaylightSec(daylightSec);
    addToHourlyLog(rain_1h);
    updateZoneManager();
    DEBUG_MSG("Rain 1h: %f\n", rain_1h);
    DEBUG_MSG("Temp: %f\n", main_temp);
    DEBUG_MSG("DaylightSec: %f hrs\n", (float)daylightSec / 60 / 60);
}

void RainAdjust::setDaylightSec(long daylightSec)
{
    _daylightSec = daylightSec;
}

void RainAdjust::updateZoneManager()
{
    _zoneManager->setWeatherAdjustRate(getRainAdjust() * getSunlightAdjust());
}

float RainAdjust::getSunlightAdjust()
{
    // If weather not fetched from server
    if (_daylightSec == -1)
    {
        return 1;
    }

    float adjust = (float)(_daylightSec - MIN_SUNLIGHT_SEC) / (float)(MAX_SUNLIGHT_SEC - MIN_SUNLIGHT_SEC);

    adjust = adjust > 1 ? 1 : adjust;
    adjust = adjust < 0 ? 0 : adjust;

    DEBUG_MSG("Daylight adjust: %f\n", adjust);
    return adjust;
}

float RainAdjust::getRainAdjust()
{
    unsigned int rainFall = sumWeekLog();
    float adjust = (_targetLevel - rainFall) / _targetLevel;

    DEBUG_MSG("Rain adjust: %f\n", adjust);
    return adjust <= 1 ? adjust : 0;
}
