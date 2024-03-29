#include "Api.h"

bool postRequestOk(ESP8266WebServer *server)
{
    return server->hasArg("plain");
}

void sendStatus(ESP8266WebServer *server, Status status)
{
    StaticJsonDocument<STATUS_CAPACITY> doc;

    JsonArray days = doc.createNestedArray("days");
    for (int i = 0; i < NUM_OF_WEEKDAYS; i++)
    {
        days.add(status.days[i]);
    }

    JsonArray durations = doc.createNestedArray("durations");
    for (int i = 0; i < NUM_OF_ZONES; i++)
    {
        durations.add(status.durations[i]);
    }

    JsonArray startTime = doc.createNestedArray("startTime");
    for (int i = 0; i < NUM_OF_HH_MIN; i++)
    {
        startTime.add(status.startTime[i]);
    }

    doc["wateringMode"] = status.wateringMode;
    doc["isWatering"] = status.isWatering;
    doc["activeZone"] = status.activeZoneIndex;
    doc["timeRemaining"] = status.timeRemainingMs;
    doc["weatherAdjustRate"] = status.weatherAdjustRate;

    String buffer;
    serializeJson(doc, buffer);
    server->send(200, "text/json", buffer);
}

Settings receiveSettings(ESP8266WebServer *server)
{
    String postBody = server->arg("plain");
    DEBUG_MSG("POST body:\n\t%s\n", postBody.c_str());

    StaticJsonDocument<SETTINGS_CAPACITY> doc;
    DEBUG_MSG("StaticJsonDocument capacity created: %d\n", doc.capacity());

    DeserializationError error = deserializeJson(doc, postBody);

    JsonObject postObj = doc.as<JsonObject>();

    if (error)
    {
        DEBUG_MSG("Error parsing JSON: %s\n", error.c_str());
        postObj["error"] = JsonParse;
    }
    else
    {
        postObj["error"] = 0;
    }

    DEBUG_MSG("Parsing data\n");
    return parseSettings(postObj);
}

void sendError(ESP8266WebServer *server, String error)
{
    server->send(400, "text/json", error);
}

void sendOk(ESP8266WebServer *server)
{
    server->send(200, "text/json", "{\"Payload\": \"OK\"}");
}

Settings parseSettings(JsonObject postObj)
{
    Settings settings;

    settings.validated = postObj["error"] ? false : true;
    DEBUG_MSG("settings.validated = %s\n", postObj["error"] ? "false" : "true");

    for (int i = 0; i < NUM_OF_WEEKDAYS; i++)
    {
        settings.days[i] = postObj["days"][i];
    }

    for (int i = 0; i < NUM_OF_ZONES; i++)
    {
        settings.durations[i] = postObj["durations"][i];
    }

    for (int i = 0; i < NUM_OF_HH_MIN; i++)
    {
        settings.startTime[i] = postObj["startTime"][i];
    }
    return settings;
}
