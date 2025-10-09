#include <Arduino.h>
#include "JsonParser.h"
#include "constants.h"
#include "config.h"
#include "sensor_data_types.h"
#include <etl/string.h>
#include <etl/vector.h>

int             counter{0};
etl::string<32> hello{};
etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE> testReadings;

void setup() {
    testReadings.push_back({ 1726995605, 25, 50 });
    testReadings.push_back({ 1726995610, 30, 55 });
    hello.append("Hello ETL Arduino No ");

    Serial.begin(115200);
    delay(2000);

    Serial.print(hello.c_str());
    Serial.println(counter);
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