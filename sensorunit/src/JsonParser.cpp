/**
 * @file JsonParser.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the JsonParser class for composing JSON payloads from sensor data.
 *
 * This source file contains the implementation of methods declared in JsonParser.h.
 * It uses ArduinoJson and ETL containers to build structured JSON strings from batches
 * of sensor readings.
 * @date 2025-10-08
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "JsonParser.h"

etl::string<MAX_JSON_SIZE>
JsonParser::composeSensorSnapshotGroup(etl::vector<ca_sensorunit_reading, MAX_BATCH_SIZE>& readings,
                                       const char*                                         uuid) {
    StaticJsonDocument<MAX_JSON_DOC_SIZE> doc;
    doc["sensor_unit_id"]   = uuid;
    JsonArray readingsArray = doc["readings"].to<JsonArray>();

    for (auto reading : readings) {
        JsonObject readingsEntry     = readingsArray.createNestedObject();
        readingsEntry["timestamp"]   = reading.timestamp;
        readingsEntry["temperature"] = reading.temperature;
        readingsEntry["humidity"]    = reading.humidity;
    }

    char rawOutput[MAX_JSON_SIZE];
    serializeJson(doc, rawOutput);
    return etl::string<MAX_JSON_SIZE>(rawOutput);
}