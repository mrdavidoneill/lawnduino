#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "Secrets.h"
#include <ProgramManager.h>
#include <TimeManager.h>
#include <Router.h>
#include <WiFiUdp.h>

#include <RainAdjust.h>

// Debugger
#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

int PINS[NUM_OF_ZONES] = {2, 3};

ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.es.pool.ntp.org", TZ_OFFSET_S);

WiFiClient client;
HTTPClient http;

ProgramManager programmanager;
Router webrouter;
TimeManager timemanager;

RainAdjust rainAdjust;

boolean isConnected();
void connectWiFi();
void awaitConnection();

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("LawnManager");
  connectWiFi();
  timemanager = TimeManager(&timeClient);
  programmanager = ProgramManager(PINS, &timemanager);
  rainAdjust = RainAdjust(&client, &http, &programmanager);
  webrouter = Router(&server, &programmanager);
  webrouter.begin();
  server.begin();
}

void loop()
{
  server.handleClient();
  timemanager.manage();
  programmanager.manage();
  rainAdjust.manage();
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
