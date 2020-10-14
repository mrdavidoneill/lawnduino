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
    _server->on(STOP, [=]() { Router::handleStop(); });
}

void Router::handleAuto()
{
    _zonemanager->setMode(MODE_AUTO);
    _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}

void Router::handleManual()
{
    _zonemanager->setMode(MODE_MANUAL);
    _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}

void Router::handleNext()
{
    _zonemanager->nextZone();
    _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}

void Router::handleSave()
{
    if (postRequestOk(_server))
    {
        _zonemanager->loadSettings(receiveSettings(_server));
        _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
    }
    else
    {
        sendError(_server, "Bad request");
    }
}

void Router::handleStart()
{
    _zonemanager->startWatering();
    _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}

void Router::handleStop()
{
    _zonemanager->stopWatering();
    _server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}