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
        ESP_LOGE(TAG, "Failed to parse JSON");
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