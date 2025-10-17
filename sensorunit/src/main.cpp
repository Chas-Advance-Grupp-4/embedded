#include <Arduino.h>
#include "logging.h"
#include "JsonParser.h"
#include "constants.h"
#include "config.h"
#include "sensor_data_types.h"
#include "communication_data_types.h"
#include "ConnectionManager.h"
#include "RestClient.h"
#include <etl/string.h>
#include <etl/vector.h>

etl::vector<CaSensorunitReading, json_config::max_batch_size> testReadings;
RestClient restClient(TEST_SERVER_IP);
ConnectionManager connectionManager(restClient);

void setup() {
    testReadings.push_back({ 1726995605, 25, 50 });
    testReadings.push_back({ 1726995610, 30, 55 });

    Serial.begin(115200);
    delay(2000);

    connectionManager.init();
    connectionManager.connect();

    
    RestResponse response = restClient.getTo("/time");
    
    LOG_INFO("MAIN", "GET response status: %d", response.status);
    LOG_INFO("MAIN", "GET response payload: %s", response.payload.c_str());

    RestResponse postResponse = restClient.postTo("/connect", "\"sensor_unit_id\":\"12345-67890\"");
    LOG_INFO("MAIN", "POST response status: %d", postResponse.status);
    LOG_INFO("MAIN", "POST response payload: %s", postResponse.payload.c_str());

    ConnectResponse test = JsonParser::parseConnectResponse(R"({"status":"connected","sensor_id": 1}")");
    LOG_INFO("MAIN", "Connected: %s", test.connected ? "true" : "false");
    LOG_INFO("MAIN", "Sensor Id: %d", test.sensorId); 
    LOG_INFO("MAIN", "Setup done");
}

void loop() {
    delay(2000);
}