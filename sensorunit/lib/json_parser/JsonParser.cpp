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

etl::string<json_config::max_json_size>
JsonParser::composeSensorSnapshotGroup(etl::vector<ca_sensorunit_reading, json_config::max_batch_size>& readings,
                                       const char*                                         uuid) {
    StaticJsonDocument<json_config::max_json_doc_size> doc;
    doc["sensor_unit_id"]   = uuid;
    JsonArray readingsArray = doc["readings"].to<JsonArray>();

    for (auto reading : readings) {
        JsonObject readingsEntry     = readingsArray.createNestedObject();
        readingsEntry["timestamp"]   = reading.timestamp;
        readingsEntry["temperature"] = reading.temperature;
        readingsEntry["humidity"]    = reading.humidity;
    }

    char rawOutput[json_config::max_json_size];
    serializeJson(doc, rawOutput);
    return etl::string<json_config::max_json_size>(rawOutput);
}