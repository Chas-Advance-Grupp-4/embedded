/**
 * @file JsonParser.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief 
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "JsonParser.h"
#include "cJSON.h"
#include "esp_log.h"
#include <memory>

static const char* TAG = "JsonParser";

/**
 * @brief parse JSON coming from sensor units, extract and return the data
 * 
 * The resulting std::vector contains readings from one sensor unit
 * 
 * @param json 
 * @return std::vector<ca_sensorunit_snapshot> 
 */
std::vector<ca_sensorunit_snapshot>
JsonParser::parseSensorSnapshotGroup(const std::string& json) {
    std::vector<ca_sensorunit_snapshot> snapshots;

    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return snapshots;
    }

    // UUID
    cJSON* uuidItem = cJSON_GetObjectItem(root, "uuid");
    if (!cJSON_IsString(uuidItem) || !uuidItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'uuid'");
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

/**
 * @brief Take a std::map of readings and compose a JSON payload
 * 
 * The resulting JSON is sorted and grouped on (UNIX) timestamps
 * 
 * @param readings 
 * @param controlunit_uuid 
 * @return std::string 
 */
std::string JsonParser::composeGroupedReadings(
    const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings,
    const std::string& controlunit_uuid) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(
        root, "device_uuid", controlunit_uuid.c_str());

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
                cJSON_AddStringToObject(
                    unitObj, "uuid", snapshot.uuid->toString().c_str());
            } else {
                cJSON_AddStringToObject(unitObj, "uuid", "unknown");
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
    cJSON_free(jsonStr);
    cJSON_Delete(root);

    return result;
}

/**
 * @brief Parse JSON message coming from backend regarding connecting sensor units
 * 
 * @param json 
 * @param type valid types: requestType::CONNECT, requestType::DISCONNECT
 * @return SensorConnectRequest
 */
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
    // ? Add check if uuid is valid ?
    cJSON* uuidItem = cJSON_GetObjectItem(root, "sensoruuid");
    if (!cJSON_IsString(uuidItem) || !uuidItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'sensoruuid'");
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

/**
 * @brief Takes a Sensor response struct and composes a JSON payload for backend communication
 * 
 * @param response 
 * @param controlunit_uuid 
 * @return std::string 
 */
std::string
JsonParser::composeSensorConnectResponse(const SensorConnectResponse& response,
                                         const std::string& controlunit_uuid) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "controlunit_uuid", controlunit_uuid.c_str());

    // Sensor UUID
    if (response.sensorUuid && response.sensorUuid->isValid()) {
        cJSON_AddStringToObject(
            root, "sensor_uuid", response.sensorUuid->toString().c_str());
    } else {
        cJSON_AddStringToObject(root, "sensor_uuid", "unknown");
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

/**
 * @brief  Parse JSON message coming from backend regarding connecting Driver
 * 
 * @param json 
 * @param type 
 * @return DriverConnectRequest 
 */
DriverConnectRequest
JsonParser::parseDriverConnectRequest(const std::string& json,
                                      requestType        type) {
    DriverConnectRequest request;

    cJSON* root = cJSON_Parse(json.c_str());
    if (!root) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", json.c_str());
        return request;
    }

    // DriverId
    const cJSON* driverIdItem = cJSON_GetObjectItem(root, "driver_id");
    if (!cJSON_IsNumber(driverIdItem)) {
        ESP_LOGE(TAG, "Missing or invalid 'driver_id'");
        cJSON_Delete(root);
        return request;
    }
    double rawValue = driverIdItem->valuedouble;
    if (rawValue < 0 || rawValue > std::numeric_limits<uint32_t>::max()) {
        ESP_LOGE(TAG, "'driver_id' out of range");
        cJSON_Delete(root);
        return request;
    }
    uint32_t driverId = static_cast<uint32_t>(rawValue);

    // Token
    const cJSON* tokenItem = cJSON_GetObjectItem(root, "token");
    if (!cJSON_IsString(tokenItem) || !tokenItem->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'token'");
        cJSON_Delete(root);
        return request;
    }
    std::string token{tokenItem->valuestring};

    request.driverId = driverId;
    request.token    = token;
    request.request  = type;

    cJSON_Delete(root);
    return request;
}

/**
 * @brief Takes a Driver response struct and composes a JSON payload for backend communication
 * 
 * @param response 
 * @param controlunit_uuid 
 * @return std::string 
 */
std::string
JsonParser::composeDriverConnectResponse(const DriverConnectResponse& response,
                                         const std::string& controlunit_uuid) {

    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "controlunit_uuid", controlunit_uuid.c_str());

    // Driver ID
    if (response.driverId != 0) {
        cJSON_AddNumberToObject(root, "driver_id", response.driverId);
    } else {
        ESP_LOGE(TAG, "Missing or invalid 'driver_id'");
        cJSON_AddNullToObject(root, "driver_id");
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

/**
 * @brief Generate generic error message for communication
 *
 * TODO: Discuss error handling with backend and expected responses
 *
 * @param message The string that is added to the message field
 * @return std::string
 */
std::string
JsonParser::composeErrorResponse(const std::string& message,
                                 const std::string& controlunit_uuid) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — Test with: f47ac10b-58cc-4372-a567-0e02b2c3d479
    cJSON_AddStringToObject(root, "controlunit_uuid", controlunit_uuid.c_str());
    cJSON_AddStringToObject(root, "status", "error");
    cJSON_AddStringToObject(root, "message", message.c_str());

    char*       jsonStr = cJSON_PrintUnformatted(root);
    std::string result(jsonStr);
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}