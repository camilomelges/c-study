#include <stdio.h>
#include <time.h>
#include "emitter.h"

/** Default defs **/
typedef char *string;
typedef struct DeviceSensorData
{
  string uniqueId;
  string deviceId;
  string sensorId;
} DeviceSensorData;

typedef struct Log
{
  string id;
  string key;
  string value;
  string timestamp;
  struct DeviceSensorData deviceSensorData;
} Log;
/** Default defs **/

/** Driver handle **/
string getWeight() { return "100"; }
string getSize() { return "100"; }
string getGender() { return "M"; }
struct mock_driver
{
  string (*getWeight)();
  string (*getSize)();
  string (*getGender)();
} mock_driver;
/** Driver handle **/

struct Logger
{
  void (*trigger)(void *data);
  void (*print)(void *data);
} logger;

void print(void *data) {
  printf("here!");
}

void trigger(void *data)
{
  emitter_t *emitter = emitter_new();
  struct DeviceSensorData *deviceSensorData = (struct DeviceSensorData *)data;

  string logs[6] = {};
  logs[0] = "weight";
  logs[1] = mock_driver.getWeight();

  logs[2] = "size";
  logs[3] = mock_driver.getSize();

  logs[4] = "gender";
  logs[5] = mock_driver.getGender();

  for (int i = 0; i < sizeof(logs); i = i + 2)
  {
    time_t seconds = time(NULL);
    char epoch[15 + sizeof(char)];

    sprintf(epoch, "%ld", (long) seconds);

    struct Log log;
    log.id = "";
    log.key = logs[i];
    log.value = logs[i + 1];
    log.timestamp = epoch;
    log.deviceSensorData = *deviceSensorData;

    emitter_emit(emitter, "print", &deviceSensorData);
  }
}

void init()
{
  mock_driver.getWeight = &getWeight;
  mock_driver.getSize = &getSize;
  mock_driver.getGender = &getGender;

  emitter_t *emitter = emitter_new();

  logger.trigger = &trigger;
  logger.print = &print;

  emitter_on(emitter, "trigger", logger.trigger);
  emitter_on(emitter, "print", logger.print);

  struct DeviceSensorData deviceSensorData;
  deviceSensorData.deviceId = "123";
  deviceSensorData.sensorId = "123";
  deviceSensorData.uniqueId = "123";

  emitter_emit(emitter, "trigger", &deviceSensorData);
}

int main(void)
{
  init();
  return 0;
}