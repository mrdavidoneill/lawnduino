#include <Arduino.h>
#include <unity.h>
#include <RainAdjust.h>

RainAdjust rainAdjust;
float hourlyLog[HOURLY_LOG_LENGTH];
float weekLog[WEEK_LOG_LENGTH];

void setUp()
{
    // set stuff up here
}

// void tearDown(void) {
// // clean stuff up here
// }

void testRainAdjustInitial()
{
    rainAdjust = RainAdjust();
    TEST_ASSERT_EQUAL(1, rainAdjust.getRainAdjust());
}

void testSumHourlyLog()
{

    for (int i = 0; i < HOURLY_LOG_LENGTH; i++)
    {
        hourlyLog[i] = 1;
    }

    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(HOURLY_LOG_LENGTH, rainAdjust.sumHourlyLog());
}

void testSumWeekLog()
{

    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        weekLog[i] = 1;
    }

    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(WEEK_LOG_LENGTH, rainAdjust.sumWeekLog());
}

void testRainAdjustAtLength()
{
    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        weekLog[i] = 1;
    }

    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(WEEK_LOG_LENGTH, rainAdjust.sumWeekLog());
    TEST_ASSERT_EQUAL(0, rainAdjust.getRainAdjust());
}

void testRainAdjustAtTarget()
{
    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        weekLog[i] = 0;
    }

    // 1st day target rain.  No rain any other day
    weekLog[0] = DEFAULT_TARGET;
    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(0, rainAdjust.getRainAdjust());

    // last day target rain.  No rain any other day
    weekLog[0] = 0;
    weekLog[WEEK_LOG_LENGTH - 1] = DEFAULT_TARGET;
    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(0, rainAdjust.getRainAdjust());
}

void testRainAdjustAtNoRain()
{
    for (int i = 0; i < WEEK_LOG_LENGTH; i++)
    {
        weekLog[i] = 0;
    }

    rainAdjust = RainAdjust(hourlyLog, weekLog);
    TEST_ASSERT_EQUAL(1, rainAdjust.getRainAdjust());
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(testRainAdjustInitial);
    RUN_TEST(testRainAdjustAtNoRain);
    RUN_TEST(testRainAdjustAtTarget);
    RUN_TEST(testRainAdjustAtLength);
    RUN_TEST(testSumHourlyLog);
    RUN_TEST(testSumWeekLog);
}

void loop()
{
    UNITY_END(); // stop unit testing
}
