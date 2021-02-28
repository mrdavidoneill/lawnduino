
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "ProgramManager.h"

ProgramManager::ProgramManager(){};

ProgramManager::ProgramManager(int pins[NUM_OF_ZONES], TimeManager *timeManager)
{
    DEBUG_MSG("Creating ProgramManager\n");
    for (int i = 0; i < NUM_OF_PROGRAMS; i++)
    {
        _zoneManagers[i] = ZoneManager(pins, timeManager);
    }
}

bool ProgramManager::loadProgramSettings(int program, Settings settings)
{
    DEBUG_MSG("Loading settings for %d\n", program);
    return _zoneManagers[program].loadSettings(settings);
};

Status ProgramManager::getProgramStatus(int program)
{
    DEBUG_MSG("Getting status of %d\n", program);
    return _zoneManagers[program].getStatus();
}

void ProgramManager::setMode(WateringMode mode)
{
    DEBUG_MSG("Setting modes\n");
    for (int i = 0; i < NUM_OF_PROGRAMS; i++)
    {
        _zoneManagers[i].setMode(mode);
    }
}

void ProgramManager::setWeatherAdjustRate(float weatherAdjustRate)
{
    for (int i = 0; i < NUM_OF_PROGRAMS; i++)
    {
        _zoneManagers[i].setWeatherAdjustRate(weatherAdjustRate);
    }
}

bool ProgramManager::shouldStartWatering()
{
    return zoneManagerShouldStart() > -1;
}

int ProgramManager::zoneManagerShouldStart()
{
    for (int i = 0; i < NUM_OF_PROGRAMS; i++)
    {
        if (_zoneManagers[i].shouldStartWatering())
        {
            return i;
        }
    }
    return -1;
}

bool ProgramManager::isWatering()
{
    return _currentZoneManagerIndex > -1;
}

// if is watering, if not zoneManager[current].isWatering then start zoneManager[0]
// if not watering, if any zone manager should startWatering, start them
// @TODO Maybe no need for NextZoneManager(). They're not consecutive
//   Instead, just loop over each and start one independently if needed
// Create function to get zoneManager who shouldStartWatering
//     Maybe change start watering to accept a program index?
void ProgramManager::updateZoneManagers()
{
    if (isWatering())
    {
        if (!_zoneManagers[getCurrentIndex()].isWatering())
        {
            DEBUG_MSG("Finished program\n");
            stopWatering();
        }
    }
    else
    {
        int zoneToStart = zoneManagerShouldStart();
        if (zoneToStart > -1)
        {
            DEBUG_MSG("New program\n");
            setCurrentIndex(zoneToStart);
            _zoneManagers[getCurrentIndex()].startWatering();
        }
    }
}

void ProgramManager::resetCurrentIndex()
{
    _currentZoneManagerIndex = -1;
}

void ProgramManager::setCurrentIndex(int index)
{
    _currentZoneManagerIndex = index;
}

int ProgramManager::getCurrentIndex()
{
    return _currentZoneManagerIndex;
}

void ProgramManager::startWatering()
{
    int program = 0;
    DEBUG_MSG("Starting program %d...\n", program);
    _zoneManagers[program].setShouldStartWatering(true);
}

void ProgramManager::stopWatering()
{
    if (isWatering())
    {
        if (_zoneManagers[getCurrentIndex()].isWatering())
        {
            DEBUG_MSG("Stopping program %d...\n", _currentZoneManagerIndex);
            _zoneManagers[getCurrentIndex()].stopWatering();
        }
        resetCurrentIndex();
        DEBUG_MSG("Program stopped OK\n");
    };
}

void ProgramManager::nextZone()
{
    if (isWatering())
    {
        _zoneManagers[getCurrentIndex()].nextZone();
    };
}

void ProgramManager::manage()
{
    for (int i = 0; i < NUM_OF_PROGRAMS; i++)
    {
        _zoneManagers[i].manage();
    }
    updateZoneManagers();
};
