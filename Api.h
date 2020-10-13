/*
  Api.h - Library for a REST API.
  Created by David A. O'Neill, October 12, 2010.
  Released into the public domain.
*/
#ifndef Api_h
#define Api_h

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include "Constants.h"
#include "Errors.h"

void sendError(ESP8266WebServer *server, String error);
bool postRequestOk(ESP8266WebServer *server);
Settings parseSettings(JsonObject);
Settings receiveSettings(ESP8266WebServer *server);

#endif
