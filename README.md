# SETUP

## Set number of zones to control:

Inside `Config/config.h`,

```cpp
#define NUM_OF_ZONES 6
```

## Debugging:

In `platform.ini` add:

```ini
build_flags = -D DEBUG_ESP_PORT=Serial
```

# API

## Start watering

http://192.168.1.81/start

## Stop watering

http://192.168.1.81/stop

## Go to next zone

http://192.168.1.81/next

## Get status

### Get status of program 0

http://192.168.1.81/status

### Get status of program 1

http://192.168.1.81/status?program=1

## JSON payloads to save settings

### Definitions:

&nbsp;&nbsp;&nbsp;&nbsp;`days` : array of true or false, where true means a watering day<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`durations` : array of milliseconds integers representing watering durations<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`startTime` : array of [hour, minutes], where hour and minutes are integers.<br/>

## startTime codes:

### Normal weekly operation

&nbsp;&nbsp;&nbsp;&nbsp;`hour` is 0-23 and `minutes` is 0-59, it represents a 24-hr clock time

### Off mode

&nbsp;&nbsp;&nbsp;&nbsp;`hour` is 24, `minutes` is ignored.

### Interval mode

&nbsp;&nbsp;&nbsp;&nbsp;`hour` is 100, and `minutes` is interval time in minutes.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Additionally, if hour = 24, it sets the zones off.

### Example:

```json
{
  "days": [1, 0, 1, 0, 1, 0, 0],
  "durations": [600000, 600000, 600000, 600000, 600000, 600000, 600000],
  "startTime": [5, 0]
}
```

## Examples calls to save settings

### Weekends starting at 05:00 for 10 mins each zone:

```powershell
curl -d "{"days":[false,false,false,false,false,true,true], "durations":[600000,600000,600000,600000,600000,600000,600000],"startTime":[5,0]}" -H "Content-Type: application/json" -X POST http://192.168.1.81/save
```

### Weekdays every 30 mins at 05:00 for 1 min each zone:

```powershell
curl -d "{"days":[true,true,true,true,true,false,false], "durations":[60000,60000,60000,60000,60000,60000,60000],"startTime":[100,30]}" -H "Content-Type: application/json" -X POST http://192.168.1.81/save
```
