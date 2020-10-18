#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "Zone.h"

Zone::Zone(){};

Zone::Zone(int pin)
{
    DEBUG_MSG("Creating Zone with pin %d ", pin);
    _pin = pin;
    _durationMs = 10 * 60 * 1000;
    _endTimestamp = 0;
    _isOn = false;

    DEBUG_MSG("and duration %d mins...", _durationMs / 60 / 1000);
    pinMode(pin, OUTPUT);
    DEBUG_MSG("Pin %d set to OUTPUT\n", pin);
}

int Zone::getPin() { return _pin; };

unsigned long Zone::getDurationMs() { return _durationMs; };
unsigned long Zone::getRemainingMs() { return _endTimestamp > millis() ? _endTimestamp - millis() : 0; };

void Zone::setDurationMs(unsigned long durationMs) { _durationMs = durationMs; };

void Zone::setStartTimestamp(unsigned long startTimestamp) { _startTimestamp = startTimestamp; };

void Zone::setEndTimestamp() { _endTimestamp = _startTimestamp + getDurationMs(); };

bool Zone::isFinished()
{
    if (_endTimestamp && (millis() > _endTimestamp))
    {
        return true;
    }
    return false;
}

void Zone::turnOn()
{
    DEBUG_MSG("Turning on pin %d for %d mins...", getPin(), _durationMs / 60 / 1000);
    setStartTimestamp(millis());
    setEndTimestamp();
    digitalWrite(_pin, HIGH);
    _isOn = true;
    DEBUG_MSG("...OK\n");
}

void Zone::turnOff()
{
    DEBUG_MSG("Turning off pin %d", _pin);
    setStartTimestamp(NULL);
    digitalWrite(_pin, LOW);
    _isOn = false;
    DEBUG_MSG("...OK\n");
};

bool Zone::getIsOn() { return _isOn; };
