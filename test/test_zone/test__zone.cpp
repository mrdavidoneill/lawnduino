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

void test_pin_number(void)
{

    TEST_ASSERT_EQUAL(5, zone.getPin());
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_pin_number);
}

void loop()
{
    UNITY_END(); // stop unit testing
}