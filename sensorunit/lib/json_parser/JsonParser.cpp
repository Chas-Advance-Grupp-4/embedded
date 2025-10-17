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
#include "logging.h"

etl::string<json_config::max_json_size> JsonParser::composeSensorSnapshotGroup(
    etl::vector<CaSensorunitReading, json_config::max_batch_size>& readings,
    const char*                                                    uuid) {
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

etl::string<json_config::max_small_json_size>
JsonParser::composeConnectRequest(const char* su_uuid) {
    StaticJsonDocument<json_config::max_small_json_doc_size> doc;
    doc["sensor_unit_id"] = su_uuid;
    char rawOutput[json_config::max_small_json_size];
    serializeJson(doc, rawOutput);
    return etl::string<json_config::max_small_json_size>{rawOutput};
}

ConnectResponse
JsonParser::parseConnectResponse(etl::string<json_config::max_small_json_size> payload) {
    const char*                                              json = payload.c_str();
    StaticJsonDocument<json_config::max_small_json_doc_size> doc;
    ConnectResponse response {false, 0};
    DeserializationError                                     error = deserializeJson(doc, json);
    if (error) {
        LOG_ERROR(TAG, "DeserializationError: %s", error.c_str());
        return response;
    }
    if (!doc.containsKey("status")) {
        LOG_WARN(TAG, "Missing 'status' field in connect response");
        return response;
    } else if (!doc.containsKey("sensor_id")) {
        LOG_WARN(TAG, "Missing 'sensor_id' field in connect response");
        return response;
    }

    response.connected = (doc["status"] == "connected");
    if (response.connected) {
        response.sensorId = (doc["sensor_id"].as<uint8_t>());
    }
    return response;
}

uint32_t
JsonParser::parseGetTimeResponse(etl::string<json_config::max_small_json_size> payload) {

    const char*                                              json = payload.c_str();
    StaticJsonDocument<json_config::max_small_json_doc_size> doc;
    DeserializationError                                     error = deserializeJson(doc, json);
    if (error) {
        LOG_ERROR(TAG, "DeserializationError: %s", error.c_str());
        return 0;
    }

    if (!doc.containsKey("timestamp")) {
        LOG_WARN(TAG, "Missing 'timestamp' field in time response");
        return 0;
    }

    uint32_t currentTime = doc["timestamp"].as<uint32_t>();

    return currentTime;
}