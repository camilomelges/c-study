#include <stdio.h>
#include <time.h>
#include <string.h>
#include "emitter.h"

/** Default defs **/
typedef struct DeviceSensorData
{
  char *uniqueId;
  char *deviceId;
  char *sensorId;
} DeviceSensorData;

typedef struct Log
{
  char *id;
  char *key;
  char *value;
  char *timestamp;
  struct DeviceSensorData deviceSensorData;
} Log;
/** Default defs **/

/** Driver handle **/
char *getWeight() { return "100"; }
char *getSize() { return "100"; }
char *getGender() { return "M"; }
struct mock_driver
{
  char *(*getWeight)();
  char *(*getSize)();
  char *(*getGender)();
} mock_driver;
/** Driver handle **/

emitter_t *emitter;

struct Logger
{
  void (*trigger)(void *data);
  void (*print)(void *data);
} logger;

void print(void *data)
{
  printf("here!");
}

void trigger(void *data)
{
  struct DeviceSensorData *deviceSensorData = (struct DeviceSensorData *)data;

  char *logs[] = {"weight", mock_driver.getWeight(), "size", mock_driver.getSize(), "gender", mock_driver.getGender()};

  int arraySize = sizeof(logs) / sizeof(logs[0]);
  for (int i = 0; i < arraySize; i = i + 2)
  {
    time_t seconds = time(NULL);
    char epoch[15 + sizeof(seconds)] = "";

    sprintf(epoch, "%ld", (long)seconds);

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
  emitter = emitter_new();
  mock_driver.getWeight = &getWeight;
  mock_driver.getSize = &getSize;
  mock_driver.getGender = &getGender;

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