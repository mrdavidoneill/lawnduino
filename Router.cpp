#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "Router.h"
#include "Api.h"

#define AUTO "/auto"
#define NEXT "/next"
#define MANUAL "/manual"
#define SAVE "/save"
#define START "/start"
#define STATUS "/status"
#define STOP "/stop"

Router::Router() {}

Router::Router(ESP8266WebServer *server, ZoneManager *zonemanager)
{
    _server = server;
    _zonemanager = zonemanager;
};

void Router::begin()
{
    _server->on(AUTO, [=]() { Router::handleAuto(); });
    _server->on(MANUAL, [=]() { Router::handleManual(); });
    _server->on(NEXT, [=]() { Router::handleNext(); });
    _server->on(SAVE, [=]() { Router::handleSave(); });
    _server->on(START, [=]() { Router::handleStart(); });
    _server->on(STATUS, [=]() { Router::handleStatus(); });
    _server->on(STOP, [=]() { Router::handleStop(); });
}

void Router::handleAuto()
{
    _zonemanager->setMode(MODE_AUTO);
    sendOk(_server);
}

void Router::handleManual()
{
    _zonemanager->setMode(MODE_MANUAL);
    sendOk(_server);
}

void Router::handleNext()
{
    _zonemanager->nextZone();
    sendOk(_server);
}

void Router::handleStatus()
{
    sendStatus(_server, _zonemanager->getStatus());
}

void Router::handleSave()
{
    if (postRequestOk(_server))
    {
        _zonemanager->loadSettings(receiveSettings(_server));
        sendOk(_server);
    }
    else
    {
        sendError(_server, "Bad request");
    }
}

void Router::handleStart()
{
    _zonemanager->startWatering();
    sendOk(_server);
}

void Router::handleStop()
{
    _zonemanager->stopWatering();
    sendOk(_server);
}