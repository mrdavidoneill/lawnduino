
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "ZoneManager.h"

ZoneManager::ZoneManager(){};

ZoneManager::ZoneManager(int pins[NUM_OF_ZONES])
{
    DEBUG_MSG("CREATING ZONEMANAGER\n");

    for (int i = 0; i < NUM_OF_ZONES; i++)
    {
        DEBUG_MSG("Index: %d: \t", i);
        _zones[i] = Zone(pins[i]);
    }
}

bool ZoneManager::loadSettings(Settings settings)
{
    if (settings.validated)
    {
        for (int i = 0; i < NUM_OF_WEEKDAYS; i++)
        {
            _wateringDays[i] = settings.days[i];
        }

        for (int i = 0; i < NUM_OF_ZONES; i++)
        {
            setZoneDuration(i, settings.durations[i] * MS_IN_MIN);
        }

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

bool ZoneManager::isTasks()
{
    return _currentZoneIndex > -1;
}

void ZoneManager::setZoneDuration(int zoneIndex, unsigned long durationMs)
{
    _zones[zoneIndex].setDurationMs(durationMs);
}

void ZoneManager::updateZones()
{

    if (isTasks())
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
}

void ZoneManager::stopWatering()
{
    DEBUG_MSG("STOPPING WATERING...");
    if (_zones[_currentZoneIndex].getIsOn())
    {
        _zones[_currentZoneIndex].turnOff();
    }
    _currentZoneIndex = -1;
    DEBUG_MSG("OK\n");
}

void ZoneManager::manage()
{
    updateZones();
};
