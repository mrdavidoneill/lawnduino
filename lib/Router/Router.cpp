#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "Router.h"

#define AUTO "/auto"
#define NEXT "/next"
#define MANUAL "/manual"
#define SAVE "/save"
#define START "/start"
#define STATUS "/status"
#define STOP "/stop"

Router::Router() {}

Router::Router(ESP8266WebServer *server, ProgramManager *programManager)
{
    _server = server;
    _programManager = programManager;
};

bool Router::isProgramNumValid()
{
    return getProgram() > -1;
}

int Router::getProgram()
{
    int program;
    if (!_server->hasArg("program"))
    {
        program = 0;
    }
    else
    {
        program = _server->arg("program").toInt();
    }
    if (0 > program || program > NUM_OF_PROGRAMS - 1)
    {
        program = -1; // Bad program
    }
    return program;
}

void Router::begin()
{
    _server->on(AUTO, [=]()
                { Router::handleAuto(); });
    _server->on(MANUAL, [=]()
                { Router::handleManual(); });
    _server->on(NEXT, [=]()
                { Router::handleNext(); });
    _server->on(SAVE, [=]()
                { Router::handleSave(); });
    _server->on(START, [=]()
                { Router::handleStart(); });
    _server->on(STATUS, [=]()
                { Router::handleStatus(); });
    _server->on(STOP, [=]()
                { Router::handleStop(); });
}

void Router::handleAuto()
{
    _programManager->setMode(MODE_AUTO);
    sendOk(_server);
}

void Router::handleManual()
{
    _programManager->setMode(MODE_MANUAL);
    sendOk(_server);
}

void Router::handleNext()
{
    _programManager->nextZone();
    sendOk(_server);
}

void Router::handleStatus()
{
    if (isProgramNumValid())
    {
        int program = getProgram();
        sendStatus(_server, _programManager->getProgramStatus(program));
        sendOk(_server);
    }
    else
    {
        sendError(_server, "Bad program");
    }
}

void Router::handleSave()
{
    if (postRequestOk(_server))
    {
        if (isProgramNumValid())
        {
            int program = getProgram();
            _programManager->loadProgramSettings(program, receiveSettings(_server));
            sendOk(_server);
        }
        else
        {
            sendError(_server, "Bad program");
        }
    }
    else
    {
        sendError(_server, "Bad request");
    }
}

void Router::handleStart()
{
    _programManager->startWatering();
    sendOk(_server);
}

void Router::handleStop()
{
    _programManager->stopWatering();
    sendOk(_server);
}