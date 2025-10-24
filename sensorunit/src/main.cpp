#include "ConnectionManager.h"
#include "ReadingProcessor.h"
#include "ReadingsDispatcher.h"
#include "RestClient.h"
#include "Scheduler.h"
#include "SensorReader.h"
#include "TimeSyncManager.h"
#include "config.h"
#include "constants.h"
#include "logging.h"
#include <Arduino.h>

RestClient         restClient(CONTROL_UNIT_IP_ADDR);
ConnectionManager  connectionManager(CONTROL_UNIT_PASSWORD, restClient);
TimeSyncManager    timeSyncManager(restClient);
Scheduler          scheduler(timeSyncManager);
SensorReader       sensorReader;
ReadingBuffer      readingBuffer;
ReadingProcessor   readingProcessor(sensorReader, timeSyncManager, readingBuffer);
ReadingsDispatcher readingsDispatcher(restClient, readingBuffer);

void setup() {
    Serial.begin(115200);
    delay(2000);

    sensorReader.init();
    connectionManager.init();
    connectionManager.connect();
    timeSyncManager.syncTime();

    LOG_INFO("MAIN", "Setup done");
}

void loop() {
    SchedulerResult triggers = scheduler.tick(connectionManager.isPairedWithControlUnit());
    if (triggers.connectTrigger) {
        connectionManager.connect();
    }
    if (triggers.readingTrigger) {
        readingProcessor.process();
    }
    if (triggers.dispatchTrigger) {
        readingsDispatcher.dispatch();
    }
    if (triggers.resyncTrigger) {
        timeSyncManager.syncTime();
    }
}