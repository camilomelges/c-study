#include <stdio.h>

/* Entity to standardize the log payload 
** The id, key(the event name), value(the event value), 
** timestamp and the device sensor data 
*/
typedef struct Log
{
  char *id;
  char *uniqueId;
  char *deviceId;
  char *sensorId;
  char *key;
  char *value;
  char *timestamp;
} Log;

/* The entity for the manager of the sensors events, maitaining the code more organized
** In this case for tests I have just two functions, to trigger the logger, the data is the DeviceSensorData
** will be used to create the log.
*/
typedef struct Logger
{
  struct Log log;
  void (*sendLog)(void *data);
} Logger;
