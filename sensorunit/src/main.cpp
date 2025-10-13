#include <Arduino.h>
#include "logging.h"
#include "JsonParser.h"
#include "constants.h"
#include "config.h"
#include "sensor_data_types.h"
#include "ConnectionManager.h"
#include <etl/string.h>
#include <etl/vector.h>

etl::vector<CaSensorunitReading, json_config::max_batch_size> testReadings;
ConnectionManager connectionManager;

void setup() {
    testReadings.push_back({ 1726995605, 25, 50 });
    testReadings.push_back({ 1726995610, 30, 55 });

    Serial.begin(115200);
    delay(2000);

    connectionManager.init();
    LOG_INFO("MAIN", "Info from Main");
    LOG_WARN("MAIN", "A Warning from Main");
    LOG_ERROR("MAIN", "An Error from Main");
    LOG_DEBUG("MAIN", "Only for Debugging");

    LOG_INFO("MAIN - JsonParser", JsonParser::composeSensorSnapshotGroup(testReadings, SENSOR_UNIT_ID).c_str());
}

void loop() {
    delay(2000);
}