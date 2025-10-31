/**
 * @file JsonParser.cpp
 * @brief Implementation of JSON parsing and composition for sensor and
 * connection data.
 *
 * This file defines the logic for converting internal data structures to and
 * from JSON format. It includes support for:
 * - Parsing grouped sensor readings from JSON
 * - Parsing backend connect request
 * - Composing responses for backend connect requests
 * - Parsing and composing JSON for Sensor Unit connect requests
 * - Handling error messages in JSON format
 *
 * Used for communication between the Control Unit both for backend services
 * and Sensor Unit. All methods are static and stateless, defined in the
 * JsonParser class.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "JsonParser.h"
#include "cJSON.h"
#include "esp_log.h"
#include <memory>

static const char* TAG = "JsonParser";

std::string JsonParser::composeStatusRequest(const std::string& controlUnitId) {
    if (controlUnitId.empty()) {
        return {};
    }
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "control_unit_id", controlUnitId.c_str());

    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}

std::vector<SensorConnectRequest>
JsonParser::parseStatusResponse(const std::string& json) {

    std::vector<SensorConnectRequest> result;
    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return result;
    }

    // Simplified - future implementations should loop through an array of commands
    // This version only ever does one push_back to result vector 
    cJSON* sensorIdItem = cJSON_GetObjectItem(root, "sensor_unit_id");
    if (!cJSON_IsString(sensorIdItem) || !sensorIdItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'sensor_unit_id'");
        cJSON_Delete(root);
        return result;
    }
    std::shared_ptr<Uuid> sensorId =
        std::make_shared<Uuid>(sensorIdItem->valuestring);
  
    cJSON* statusItem = cJSON_GetObjectItem(root, "status");
    if (!cJSON_IsString(statusItem) || !statusItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'status'");
        cJSON_Delete(root);
        return result;
    }
    std::string requestString = statusItem->valuestring;
    requestType      request;
    if (requestString == "in_transit") {
        request = requestType::CONNECT;
    } else if (requestString == "delivered") {
        request = requestType::DISCONNECT;
    } else {
        // possible other values also triggering DISCONNECT for know
        // logic for deciding and sending connect/disconnect should be in backend
        ESP_LOGI(TAG, "Generic request interpreted as DISCONNECT, status: %s", requestString.c_str());
        request = requestType::DISCONNECT;
    }
    std::string token {};   // Token is currently not used
    SensorConnectRequest command ( sensorId, request, token );
    result.push_back(command);

    ESP_LOGI(TAG, "Received backend command: Sensor ID %s ", sensorId->toString().c_str());
    ESP_LOGI(TAG, "Received backend command: %s ", requestString.c_str());

    cJSON_Delete(root);
    return result;
}

size_t JsonParser::parseBackendReadingsResponse(const std::string& json){
    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return 0;
    }
    cJSON* statusItem = cJSON_GetObjectItem(root, "status");
    if (!cJSON_IsString(statusItem) || !statusItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'status'");
        cJSON_Delete(root);
        return 0;
    }
    std::string status = statusItem->valuestring;

    if (status != "ok") {
        ESP_LOGW(TAG, "Returned 'status' not ok, status: %s", status.c_str());
        cJSON_Delete(root);
        return 0;
    }

    cJSON* savedItem = cJSON_GetObjectItem(root, "saved");
    if (!cJSON_IsNumber(savedItem) || savedItem->valuedouble < 0) {
        ESP_LOGE(TAG, "Missing or invalid 'saved'");
        cJSON_Delete(root);
        return 0;
    }

    size_t savedReadings = static_cast<size_t>(savedItem->valuedouble);
    cJSON_Delete(root);
    return savedReadings;
}


std::vector<ca_sensorunit_snapshot>
JsonParser::parseSensorSnapshotGroup(const std::string& json) {
    std::vector<ca_sensorunit_snapshot> snapshots;

    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return snapshots;
    }

    // UUID
    cJSON* uuidItem = cJSON_GetObjectItem(root, "sensor_unit_id");
    if (!cJSON_IsString(uuidItem) || !uuidItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'sensor_unit_id'");
        cJSON_Delete(root);
        return snapshots;
    }
    std::shared_ptr<Uuid> uuid = std::make_shared<Uuid>(uuidItem->valuestring);

    // Readings
    cJSON* readingsArray = cJSON_GetObjectItem(root, "readings");
    if (!cJSON_IsArray(readingsArray)) {
        ESP_LOGE(TAG, "Missing or invalid 'readings' array");
        cJSON_Delete(root);
        return snapshots;
    }

    cJSON* reading = nullptr;
    cJSON_ArrayForEach(reading, readingsArray) {
        if (!cJSON_IsObject(reading)) {
            ESP_LOGW(TAG, "Skipping non-object reading");
            continue;
        }

        cJSON* timestampItem   = cJSON_GetObjectItem(reading, "timestamp");
        cJSON* temperatureItem = cJSON_GetObjectItem(reading, "temperature");
        cJSON* humidityItem    = cJSON_GetObjectItem(reading, "humidity");

        if (!cJSON_IsNumber(timestampItem) ||
            !cJSON_IsNumber(temperatureItem) || !cJSON_IsNumber(humidityItem)) {
            ESP_LOGW(TAG, "Skipping invalid reading entry");
            continue;
        }

        ca_sensorunit_snapshot snapshot;
        snapshot.uuid        = uuid;
        snapshot.timestamp   = static_cast<time_t>(timestampItem->valuedouble);
        snapshot.temperature = temperatureItem->valuedouble;
        snapshot.humidity    = humidityItem->valuedouble;

        snapshots.push_back(snapshot);
    }

    cJSON_Delete(root);
    return snapshots;
}

std::string JsonParser::composeGroupedReadings(
    const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings,
    const std::string& controlUnitId) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "control_unit_id", controlUnitId.c_str());

    // Timestamp Groups
    cJSON* timestampGroups = cJSON_CreateArray();
    if (readings.empty()) {
        ESP_LOGI(TAG, "No new readings — sending timestamp_groups: []");
    } else {
        ESP_LOGI(
            TAG, "Composing JSON with %zu timestamp groups", readings.size());
    }
    for (const auto& [timestamp, snapshots] : readings) {
        cJSON* groupObj = cJSON_CreateObject();
        cJSON_AddNumberToObject(
            groupObj, "timestamp", static_cast<double>(timestamp));

        cJSON* sensorUnits = cJSON_CreateArray();
        for (const auto& snapshot : snapshots) {
            cJSON* unitObj = cJSON_CreateObject();
            if (snapshot.uuid && snapshot.uuid->isValid()) {
                cJSON_AddStringToObject(unitObj,
                                        "sensor_unit_id",
                                        snapshot.uuid->toString().c_str());
            } else {
                cJSON_AddStringToObject(unitObj, "sensor_unit_id", "unknown");
            }
            cJSON_AddNumberToObject(
                unitObj, "temperature", snapshot.temperature);
            cJSON_AddNumberToObject(unitObj, "humidity", snapshot.humidity);

            cJSON_AddItemToArray(sensorUnits, unitObj);
        }

        cJSON_AddItemToObject(groupObj, "sensor_units", sensorUnits);
        cJSON_AddItemToArray(timestampGroups, groupObj);
    }

    cJSON_AddItemToObject(root, "timestamp_groups", timestampGroups);

    // Convert to string and clean up
    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    // ESP_LOGI(TAG, "Resulting JSON: %s ", result.c_str());
    cJSON_free(jsonStr);
    cJSON_Delete(root);

    return result;
}

SensorConnectRequest
JsonParser::parseSensorConnectRequest(const std::string& json,
                                      requestType        type) {
    SensorConnectRequest request;
    request.sensorUuid = nullptr;

    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return request;
    }

    // UUID
    cJSON* uuidItem = cJSON_GetObjectItem(root, "sensor_unit_id");
    if (!cJSON_IsString(uuidItem) || !uuidItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'sensor_unit_id'");
        cJSON_Delete(root);
        return request;
    }
    std::shared_ptr<Uuid> sensorUuid =
        std::make_shared<Uuid>(uuidItem->valuestring);

    // Token
    cJSON* tokenItem = cJSON_GetObjectItem(root, "token");
    if (!cJSON_IsString(tokenItem) || !tokenItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'token'");
        cJSON_Delete(root);
        return request;
    }
    std::string token{tokenItem->valuestring};

    request.sensorUuid = sensorUuid;
    request.token      = token;
    request.request    = type;

    cJSON_Delete(root);
    return request;
}

std::string
JsonParser::composeSensorConnectResponse(const SensorConnectResponse& response,
                                         const std::string& controlUnitId) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "control_unit_id", controlUnitId.c_str());

    // Sensor UUID
    if (response.sensorUuid && response.sensorUuid->isValid()) {
        cJSON_AddStringToObject(
            root, "sensor_unit_id", response.sensorUuid->toString().c_str());
    } else {
        cJSON_AddStringToObject(root, "sensor_unit_id", "unknown");
    }

    // Connection Status
    cJSON_AddStringToObject(root,
                            "connection_status",
                            connectionStatusToString(response.status).c_str());

    // Convert to string and clean up
    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}

Uuid JsonParser::parseSensorunitConnectRequest(const std::string& json) {

    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return Uuid("");
    }

    cJSON* uuidItem = cJSON_GetObjectItem(root, "sensor_unit_id");
    if (!cJSON_IsString(uuidItem) || !uuidItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'sensor_unit_id'");
        cJSON_Delete(root);
        return Uuid("");
    }

    Uuid sensorUnitId(uuidItem->valuestring);
    ESP_LOGI(TAG, "Parsed Sensor Unit Id:%s ", sensorUnitId.toString().c_str());
    return sensorUnitId;
}

std::string
JsonParser::composeSensorunitStatusPayload(const std::string& status) {

    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "status", status.c_str());

    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string payload(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);

    ESP_LOGI(TAG, "Json Payload created with status %s", status.c_str());
    return payload;
}

std::string JsonParser::composeTimestampPayload(time_t now) {

    cJSON* root      = cJSON_CreateObject();
    double timestamp = static_cast<double>(now);
    cJSON_AddNumberToObject(root, "timestamp", timestamp);

    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string payload(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);

    ESP_LOGI(TAG, "Json Payload created with timestamp %.0f", timestamp);
    return payload;
}

std::string
JsonParser::composeErrorResponse(const std::string& message,
                                 const std::string& controlUnitId) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "control_unit_id", controlUnitId.c_str());
    cJSON_AddStringToObject(root, "status", "error");
    cJSON_AddStringToObject(root, "message", message.c_str());

    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}