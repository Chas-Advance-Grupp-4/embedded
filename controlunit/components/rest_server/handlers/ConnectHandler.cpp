/**
 * @file ConnectHandler.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of HTTP POST handler for sensor unit connection requests.
 *
 * Defines the logic for processing incoming POST requests containing sensor unit IDs.
 * Parses the request body, validates the identifier, checks connection status via
 * SensorUnitManager, and responds with a JSON-formatted status payload.
 *
 * Implements the processBody method from PostHandler.
 * 
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "ConnectHandler.h"
#include "JsonParser.h"
#include "esp_log.h"

ConnectHandler::ConnectHandler(const std::string& uri,
                               const SensorUnitManager& sensorUnitManager)
    : PostHandler(uri), m_sensorUnitManager(sensorUnitManager) {}

esp_err_t ConnectHandler::processBody(httpd_req_t*       req,
                                      const std::string& body) {
    ESP_LOGI(TAG, "Processing Sensorunit Connect Request");

    std::string status;
    Uuid        sensorunitId = JsonParser::parseSensorunitConnectRequest(body);

    if (!sensorunitId.isValid()) {
        ESP_LOGE(TAG, "Sensor Unit Id is invalid");
        httpd_resp_set_status(req, "400 Bad Request");
        status = "invalid";
    } else if (m_sensorUnitManager.hasUnit(sensorunitId)) {
        ESP_LOGI(
            TAG, "Sensor Unit %s connected", sensorunitId.toString().c_str());
        status = "connected";
    } else {
        ESP_LOGI(
            TAG, "Sensor Unit %s pending", sensorunitId.toString().c_str());
        status = "pending";
    }

    std::string payload = JsonParser::composeSensorunitStatusPayload(status);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, payload.c_str(), HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}