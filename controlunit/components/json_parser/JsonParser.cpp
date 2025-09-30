#include "JsonParser.h"
#include "cJSON.h"
#include "esp_log.h"
#include <memory>

static const char* TAG = "JsonParser";

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

std::string JsonParser::composeGroupedReadings(
    const std::map<time_t, std::vector<ca_sensorunit_snapshot>>& readings) {
    cJSON* root = cJSON_CreateObject();

    // Device UUID — hardcoded for the moment
    cJSON_AddStringToObject(
        root, "device_uuid", "f47ac10b-58cc-4372-a567-0e02b2c3d479");

    // Timestamp Groups
    cJSON* timestampGroups = cJSON_CreateArray();

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

std::string JsonParser::composeSensorConnectResponse(
    const SensorConnectResponse& response) {
    cJSON* root = cJSON_CreateObject();

    // Control Unit UUID — hardcoded for the moment
    cJSON_AddStringToObject(
        root, "controlunit_uuid", "f47ac10b-58cc-4372-a567-0e02b2c3d479");

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