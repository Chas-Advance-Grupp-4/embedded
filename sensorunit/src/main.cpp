#include <Arduino.h>
#include "logging.h"
#include "JsonParser.h"
#include "constants.h"
#include "config.h"
#include "sensor_data_types.h"
#include "communication_data_types.h"
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
    // connectionManager.connect();

    ConnectResponse test = JsonParser::parseConnectResponse(R"({"status":"connected","sensor_id": 1}")");
    LOG_INFO("MAIN", "Connected: %s", test.connected ? "true" : "false");
    LOG_INFO("MAIN", "Sensor Id: %d", test.sensorId); 
    LOG_INFO("MAIN", "Setup done");
}

void loop() {
    delay(2000);
}