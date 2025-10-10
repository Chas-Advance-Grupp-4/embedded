#include <Arduino.h>
#include "logging.h"
#include "JsonParser.h"
#include "constants.h"
#include "config.h"
#include "sensor_data_types.h"
#include <etl/string.h>
#include <etl/vector.h>

int             counter{0};
etl::string<32> hello{};
etl::vector<CaSensorunitReading, json_config::max_batch_size> testReadings;

void setup() {
    testReadings.push_back({ 1726995605, 25, 50 });
    testReadings.push_back({ 1726995610, 30, 55 });
    hello.append("Hello ETL Arduino No ");

    Serial.begin(115200);
    delay(2000);

    LOG_INFO("MAIN", "Hello from LOG Macro Info level");
    LOG_WARN("MAIN", "A Warning from Main");
    LOG_ERROR("MAIN", "An Error from Main");
    LOG_DEBUG("MAIN", "Only for Debugging");
    counter++;

    Serial.print("JsonParser from lib folder says: ");
    Serial.println(JsonParser::composeSensorSnapshotGroup(testReadings, SENSOR_UNIT_ID).c_str());
}

void loop() {
    delay(2000);
    Serial.print(hello.c_str());
    Serial.println(counter);
    counter++;
}