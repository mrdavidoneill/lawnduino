#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define DEBUG_MSG(...)
#endif

#include "Api.h"

// Settings struct
// {
// "days": [ true, true, true, true, true, true ],
// "durations": [ 255, 255, 255, 255, 255, 255 ],
// "startTime": [0,59]",
// "error": 0,
// }

bool postRequestOk(ESP8266WebServer *server)
{
    if (!server->hasArg("plain"))
    {
        return false;
    }
}

Settings receiveSettings(ESP8266WebServer *server)
{
    String postBody = server->arg("plain");
//    DEBUG_MSG("# POST body #\n%s\n", postBody);

    const size_t CAPACITY = JSON_ARRAY_SIZE(NUM_OF_WEEKDAYS) + JSON_ARRAY_SIZE(NUM_OF_ZONES) + JSON_ARRAY_SIZE(NUM_OF_HH_MIN) + JSON_OBJECT_SIZE(4) + 40;

    StaticJsonDocument<CAPACITY> doc;

    DeserializationError error = deserializeJson(doc, postBody);

    JsonObject postObj = doc.as<JsonObject>();

    if (error)
    {
        DEBUG_MSG("Error parsing JSON: %s\n", error.c_str());
        postObj["error"] = JsonParse;
    }

    DEBUG_MSG("Parsing data\n");
    return parseSettings(postObj);
}

void sendError(ESP8266WebServer *server, String error)
{
    server->send(400, "text/json", error);
}

Settings parseSettings(JsonObject postObj)
{
    Settings settings;

    settings.validated = postObj["error"] == 0 ? true : false;

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
