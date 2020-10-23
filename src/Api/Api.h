/*
  Api.h - Library for a REST API.
  Created by David A. O'Neill, October 12, 2020.
  Released into the public domain.
*/
#ifndef Api_h
#define Api_h

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <Config.h>
#include "Errors.h"

const size_t STATUS_CAPACITY = JSON_ARRAY_SIZE(2) + 2 * JSON_ARRAY_SIZE(6) + JSON_OBJECT_SIZE(8);
const size_t SETTINGS_CAPACITY = JSON_ARRAY_SIZE(NUM_OF_WEEKDAYS) + JSON_ARRAY_SIZE(NUM_OF_ZONES) + JSON_ARRAY_SIZE(NUM_OF_HH_MIN) + JSON_OBJECT_SIZE(4) + 40;

void sendStatus(ESP8266WebServer *server, Status status);
void sendOk(ESP8266WebServer *server);
void sendError(ESP8266WebServer *server, String error);
bool postRequestOk(ESP8266WebServer *server);
Settings parseSettings(JsonObject);
Settings receiveSettings(ESP8266WebServer *server);

#endif
