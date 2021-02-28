#ifndef Constants_h
#define Constants_h

// USER SET DEFINITIONS:
#define NUM_OF_ZONES 6

// Constants
#define NUM_OF_WEEKDAYS 7
#define NUM_OF_PROGRAMS 2
#define NUM_OF_HH_MIN 2

// Time mode codes
#define HH_OFF_CODE 24       // Off mode
#define HH_INTERVAL_CODE 100 // Interval mode

// Utility definitions
#define MS_IN_SEC (1000)
#define MS_IN_MIN (MS_IN_SEC * 60)

#define SEC_IN_MIN (60)
#define SEC_IN_HOUR (SEC_IN_MIN * 60)

// Frequency to sync to online clock
#define TIME_SYNC_RATE_MS (MS_IN_MIN * 3)

// User timezone in seconds
#define TZ_OFFSET_S (SEC_IN_HOUR * 2)

enum WateringMode
{
    MODE_MANUAL,
    MODE_AUTO,
};

typedef struct Settings
{
    bool days[NUM_OF_WEEKDAYS];
    unsigned long durations[NUM_OF_ZONES];
    short startTime[NUM_OF_HH_MIN];
    bool validated;
} Settings;

typedef struct Status
{
    bool days[NUM_OF_WEEKDAYS];
    unsigned long durations[NUM_OF_ZONES];
    short startTime[NUM_OF_HH_MIN];
    WateringMode wateringMode;
    bool isWatering;
    short activeZoneIndex;
    unsigned long timeRemainingMs;
    float weatherAdjustRate;
} Status;

// Debugger
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#endif
