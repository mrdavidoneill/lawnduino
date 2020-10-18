#ifndef Constants_h
#define Constants_h

#define NUM_OF_WEEKDAYS 7
#define NUM_OF_ZONES 2
#define NUM_OF_HH_MIN 2

#define MS_IN_SEC (1000)
#define MS_IN_MIN (MS_IN_SEC * 60)

#define SEC_IN_MIN (60)
#define SEC_IN_HOUR (SEC_IN_MIN * 60)

#define TIME_SYNC_RATE_MS (MS_IN_MIN * 3)
#define TZ_OFFSET_S (SEC_IN_HOUR * 2)

enum WateringMode
{
    MODE_MANUAL,
    MODE_AUTO,
};

typedef struct Settings
{
    bool days[NUM_OF_WEEKDAYS];
    short durations[NUM_OF_ZONES];
    short startTime[NUM_OF_HH_MIN];
    bool validated;
} Settings;

typedef struct Status
{
    bool days[NUM_OF_WEEKDAYS];
    short durations[NUM_OF_ZONES];
    short startTime[NUM_OF_HH_MIN];
    WateringMode wateringMode;
    bool isWatering;
    short activeZoneIndex;
    unsigned long timeRemainingMs;
} Status;

#endif
