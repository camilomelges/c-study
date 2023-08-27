#include <stdio.h>
#include "describe.h"
#include "sensor-listener.c"

char *pattern = "{ id: %s, uniqueId: %s, deviceId: %s, sensorId: %s, %s: %s, timestamp: %s }";
char expectated[2000] = "";

typedef struct TestLogs
{
  struct Log logs[5];
} TestLogs;

struct TestLogs mockLogs()
{
  struct TestLogs testLogs = {{{"100", "100", "100", "100", "100", "100", "100"}, {"200", "200", "200", "200", "200", "200", "200"}, {"300", "300", "300", "300", "300", "300", "300"}, {"400", "400", "400", "400", "400", "400", "400"}, {"500", "500", "500", "500", "500", "500", "500"}}};

  return testLogs;
}

void testSensorEvent(void *srcLog)
{
  struct Log log = *(struct Log *)srcLog;
  long size = sizeof(pattern) + sizeof(log) * 2;

  char target[size];
  snprintf(target, size, pattern, log.id, log.uniqueId, log.deviceId, log.sensorId, log.key, log.value, log.timestamp);

  strcat(expectated,target);
}

void validateLogs(struct TestLogs testLogs)
{
  int logsSize = sizeof(testLogs.logs) / sizeof(testLogs.logs[0]);
  for (int i = 0; i < logsSize; i++)
  {
    long size = sizeof(pattern) + sizeof(testLogs.logs[i]) * 2;
    char target[size];
    snprintf(target, size, pattern, testLogs.logs[i].id, testLogs.logs[i].uniqueId, testLogs.logs[i].deviceId, testLogs.logs[i].sensorId, testLogs.logs[i].key, testLogs.logs[i].value, testLogs.logs[i].timestamp);

    assert(strstr(expectated, target) != NULL);
  }
}

int main()
{
  describe("sensor_event_listener")
  {
    it("Should execute sensor event 5 times with correct values")
    {
      initializeEmitter();
      createListener();

      struct TestLogs testLogs = mockLogs();

      struct Logger logger;
      logger.sendLog = testSensorEvent;

      int logsSize = sizeof(testLogs.logs) / sizeof(testLogs.logs[0]);

      emitter_on(cattleWeighing, "sendLog", &sendLog);
      for (int i = 0; i < logsSize; i++)
      {
        logger.log = testLogs.logs[i];
        emitter_emit(cattleWeighing, "sendLog", &logger.log);
      }

      validateLogs(testLogs);
    }
    return 0;
  }
}