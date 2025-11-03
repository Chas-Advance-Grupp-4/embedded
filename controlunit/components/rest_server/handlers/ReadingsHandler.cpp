/**
 * @file ReadingsHandler.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of HTTP POST handler for sensor unit readings.
 *
 * Defines the logic for processing incoming POST requests containing batched
 * sensor readings. Parses the request body, posts all individual readings,
 * checks if Sensor Unit is still connected and sends a status response
 *
 * To avoid the need for additional polling the handler
 * responds with a JSON-formatted status payload.
 * This means disconnect requests from backend can be forwarded to sensor unit
 *
 * SensorUnitManager
 *
 * Implements the processBody method from PostHandler.
 *
 * @date 2025-10-27
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "ReadingsHandler.h"
#include "JsonParser.h"
#include "esp_log.h"

ReadingsHandler::ReadingsHandler(const std::string& uri,
                                 SensorUnitManager& sensorUnitManager)
    : PostHandler(uri), m_sensorUnitManager(sensorUnitManager) {}

esp_err_t ReadingsHandler::processBody(httpd_req_t*       req,
                                       const std::string& body) {
    ESP_LOGI(TAG, "Processing Sensorunit Readings Request");

    std::string                         status{"connected"};
    std::vector<ca_sensorunit_snapshot> snapshots =
        JsonParser::parseSensorSnapshotGroup(body);

    if (snapshots.empty()) {
        ESP_LOGW(TAG, "No readings present in post to /readings");
        httpd_resp_set_status(req, "204 No Content");
        return ESP_OK;
    } else {
        ESP_LOGI(TAG, "Posting %zu readings", snapshots.size());
        if (!snapshots.at(0).uuid) {
            ESP_LOGE(TAG, "Invalid Uuid in json Payload");
            httpd_resp_set_status(req, "400 Bad Request");
        } else {
            status = (m_sensorUnitManager.hasUnit(*snapshots.at(0).uuid))
                         ? "connected"
                         : "disconnected";
            for (const auto& snapshot : snapshots) {
                m_sensorUnitManager.storeReading(snapshot);
            }
            ESP_LOGI(TAG, "Sensor Unit status: %s", status.c_str());
        }
    }

    std::string payload = JsonParser::composeSensorunitStatusPayload(status);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, payload.c_str(), HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}