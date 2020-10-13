#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "Secrets.h"
#include "ZoneManager.h"
#include "Router.h"

// Debugger
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

int PINS[NUM_OF_ZONES] = {2, 3};

ESP8266WebServer server(80);
ZoneManager zonemanager;
Router webrouter;

void setup()
{
    Serial.begin(115200);
    connectWiFi();
    zonemanager = ZoneManager(PINS);
    webrouter = Router(&server, &zonemanager);
    webrouter.begin();
    server.begin();
}

void loop()
{
    server.handleClient();
    zonemanager.manage();
}

boolean isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void connectWiFi()
{
    while (!isConnected())
    {
        DEBUG_MSG("Wifi connecting");
        awaitConnection();
    }
    DEBUG_MSG("\nWifi connected\n");
}

void awaitConnection()
{
    DEBUG_MSG("Awaiting connection...\n");
    const int maxDelayMs = 60 * 60 * 1000;
    int delayMs = 10 * 1000;
    WiFi.begin(SSID, PASSWORD);
    delay(delayMs);
    if (delayMs < maxDelayMs)
    {
        delayMs *= 10;
    }
}
