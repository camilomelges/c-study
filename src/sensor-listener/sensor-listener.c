#include "emitter.h"
#include "logger.h"
#include "sensor-listener.h"

emitter_t *cattleWeighing;

void sendLog(void *data)
{
  struct Logger *logger = (struct Logger *) data;
  logger->sendLog(&logger->log);
}

void initializeEmitter() {
  cattleWeighing = emitter_new();
}

void createListener() {
  emitter_on(cattleWeighing, "sendLog", &sendLog);
}