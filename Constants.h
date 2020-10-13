#ifndef Constants_h
#define Constants_h

#define NUM_OF_WEEKDAYS 7
#define NUM_OF_ZONES 2
#define NUM_OF_HH_MIN 2

#define MS_IN_MIN (1000 * 60)

#define TIME_SYNC_RATE_MS (1000 * 60 * 3)
#define TZ_OFFSET_S (60 * 60 * 2)

typedef struct Settings
{
    bool days[NUM_OF_WEEKDAYS];
    short durations[NUM_OF_ZONES];
    short startTime[NUM_OF_HH_MIN];
    bool validated;
} Settings;

#endif
