#include <Arduino.h>
#include <unity.h>
#include <Zone.h>

Zone zone;

void setUp(void)
{
    // set stuff up here
    zone = Zone(5);
}

// void tearDown(void) {
// // clean stuff up here
// }

void testPinNumber(void)
{
    TEST_ASSERT_EQUAL(5, zone.getPin());
}

void testIsOn(void)
{
    TEST_ASSERT_EQUAL(false, zone.getIsOn());
    zone.turnOn();
    TEST_ASSERT_EQUAL(true, zone.getIsOn());
    zone.turnOff();
    TEST_ASSERT_EQUAL(false, zone.getIsOn());
}

void testDurationMs(void)
{
    const unsigned long DURATION = 10000;
    zone.setDurationMs(DURATION);
    TEST_ASSERT_EQUAL(DURATION, zone.getDurationMs());
    zone.setDurationMs(DURATION + DURATION);
    TEST_ASSERT_EQUAL(DURATION + DURATION, zone.getDurationMs());
}

void testRemainingMs(void)
{
    zone.turnOn();
    const unsigned long REMAINING = zone.getRemainingMs();
    delay(1);
    TEST_ASSERT_LESS_THAN(REMAINING, zone.getRemainingMs());
}

void testIsFinished(void)
{
    const unsigned long DURATION = 100;
    zone.setDurationMs(DURATION);
    zone.turnOn();
    TEST_ASSERT_EQUAL(false, zone.isFinished());
    delay(DURATION + DURATION);
    TEST_ASSERT_EQUAL(true, zone.isFinished());
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(testPinNumber);
    RUN_TEST(testIsOn);
    RUN_TEST(testDurationMs);
    RUN_TEST(testRemainingMs);
    RUN_TEST(testIsFinished);
}

void loop()
{
    UNITY_END(); // stop unit testing
}