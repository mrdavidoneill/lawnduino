/*
  Router.h - Library for setting up web routes.
  Created by David A. O'Neill, October 13, 2020.
  Released into the public domain.
*/

#ifndef Router_h
#define Router_h

#include <ESP8266WebServer.h>
#include <ZoneManager.h>
#include "Api/Api.h"

class Router
{
  ESP8266WebServer *_server;
  ZoneManager *_zonemanager;

  void handleAuto();
  void handleManual();
  void handleNext();
  void handleSave();
  void handleStart();
  void handleStatus();
  void handleStop();

public:
  Router();
  Router(ESP8266WebServer *server, ZoneManager *zonemanager);
  void begin();
};

#endif