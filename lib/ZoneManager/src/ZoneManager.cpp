
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "ZoneManager.h"

ZoneManager::ZoneManager(){};

ZoneManager::ZoneManager(int pins[NUM_OF_ZONES], TimeManager *timeManager)
{
    DEBUG_MSG("Creating ZoneManager\n");

    for (int i = 0; i < NUM_OF_ZONES; i++)
    {
        DEBUG_MSG("Index: %d: \t", i);
        _zones[i] = Zone(pins[i]);
    }

    _timeManager = timeManager;
}

bool ZoneManager::loadSettings(Settings settings)
{
    DEBUG_MSG("Loading settings\n");
    if (settings.validated)
    {
        for (int i = 0; i < NUM_OF_WEEKDAYS; i++)
        {
            DEBUG_MSG("Day %d: Watering? %d\n", i, settings.days[i]);
            _wateringDays[i] = settings.days[i];
        }

        for (int i = 0; i < NUM_OF_ZONES; i++)
        {
            DEBUG_MSG("Zone %d: Duration: %d mins\n", i, settings.durations[i] / MS_IN_MIN);
            setZoneDuration(i, settings.durations[i]);
        }

        DEBUG_MSG("Start time %d:%d\n", settings.startTime[0], settings.startTime[1]);
        for (int i = 0; i < NUM_OF_HH_MIN; i++)
        {
            _startTime[i] = settings.startTime[i];
        }
        return true;
    }
    else
    {
        return false;
    }
};

Status ZoneManager::getStatus()
{
    Status status;
    for (int i = 0; i < NUM_OF_WEEKDAYS; i++)
    {
        status.days[i] = _wateringDays[i];
    }
    for (int i = 0; i < NUM_OF_ZONES; i++)
    {
        status.durations[i] = getZoneDurationMs(i);
    }
    for (int i = 0; i < NUM_OF_HH_MIN; i++)
    {
        status.startTime[i] = _startTime[i];
    }

    status.wateringMode = _mode;
    status.isWatering = isWatering();
    status.activeZoneIndex = _currentZoneIndex;
    status.timeRemainingMs = (_currentZoneIndex > -1) ? getZoneMsRemaining(_currentZoneIndex) : 0;
    status.weatherAdjustRate = _weatherAdjustRate;

    return status;
}

bool ZoneManager::isWatering()
{
    return _currentZoneIndex > -1;
}

void ZoneManager::setZoneDuration(int zoneIndex, unsigned long durationMs)
{
    _zones[zoneIndex].setDurationMs(durationMs);
}

unsigned long ZoneManager::getZoneDurationMs(int zoneIndex)
{
    return _zones[zoneIndex].getDurationMs();
}

unsigned long ZoneManager::getZoneMsRemaining(int zoneIndex)
{
    return _zones[zoneIndex].getRemainingMs();
}

void ZoneManager::setMode(WateringMode mode)
{
    if (mode == _mode)
    {
        DEBUG_MSG("Mode unchanged from %d to %d\n", _mode, mode);
        return;
    }

    if (isWatering())
    {
        stopWatering();
    }

    DEBUG_MSG("Mode changed from %d to %d\n", _mode, mode);
    _mode = mode;
}

void ZoneManager::updateZones()
{
    if (isWatering())
    {
        if (!(_zones[_currentZoneIndex].getIsOn()))
        {
            DEBUG_MSG("NEW TASK\n");
            _zones[_currentZoneIndex].turnOn();
        }
        if (_zones[_currentZoneIndex].isFinished())
        {
            DEBUG_MSG("FINISHED TASK\n");
            nextZone();
        }
    };
}

void ZoneManager::nextZone()
{
    if (_currentZoneIndex < 0)
    {
        return;
    };

    _zones[_currentZoneIndex].turnOff();

    if (_currentZoneIndex < (NUM_OF_ZONES - 1))
    {
        _currentZoneIndex++;
    }
    else
    {
        stopWatering();
    }
}

void ZoneManager::startWatering()
{
    DEBUG_MSG("STARTING WATERING...\n");
    _currentZoneIndex = 0;

    // Don't sync if watering - Danger of bad server sync
    _timeManager->setIsOnline(false);
}

void ZoneManager::stopWatering()
{
    DEBUG_MSG("STOPPING WATERING...");
    if (_zones[_currentZoneIndex].getIsOn())
    {
        _zones[_currentZoneIndex].turnOff();
    }
    _currentZoneIndex = -1;
    _timeManager->setIsOnline(true);
    DEBUG_MSG("OK\n");
}

void ZoneManager::manage()
{
    if (_mode == MODE_AUTO)
    {
        autoMode();
    }
    updateZones();
};

void ZoneManager::autoMode()
{
    if (isWatering())
    {
        return;
    }

    if (!_wateringDays[_timeManager->getDaysSinceSunday()])
    {
        return;
    }

    if (!_timeManager->isTime(_startTime[0], _startTime[1]))
    {
        return;
    }

    startWatering();
}

void ZoneManager::setWeatherAdjustRate(float weatherAdjustRate)
{
    _weatherAdjustRate = weatherAdjustRate;
}

float ZoneManager::getWeatherAdjustRate()
{
    return _weatherAdjustRate;
}