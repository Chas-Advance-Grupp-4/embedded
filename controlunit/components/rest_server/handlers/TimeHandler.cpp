/**
 * @file HelloHandler.cpp
 * @brief Implementation of HelloHandler, a simple GET endpoint that returns a
 * greeting.
 *
 * Defines the logic for responding to HTTP GET requests with a static greeting
 * message. Inherits from GetHandler and overrides the `process()` method to
 * send a plain-text response.
 *
 * Useful for testing server availability or providing a basic health check.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "TimeHandler.h"
#include "esp_log.h"
#include "time.h"

TimeHandler::TimeHandler(const std::string& uri, TimeSyncManager& timeSyncManager)
    : GetHandler {uri} , m_timeSyncManager { timeSyncManager } {}

esp_err_t TimeHandler::process(httpd_req_t* req) {

    if (!m_timeSyncManager.isTimeSynced()) {
            ESP_LOGE(TAG, "failed GET /time request. Internal time is not synced");
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Control Unit time is not synced");
            return ESP_FAIL;
    }
    
    time_t now;
    time(&now);
    ESP_LOGI(TAG,"Control Unit timestamp: %lld", static_cast<long long>(now));    

    char response[64];
    uint32_t truncatedTimestamp = static_cast<uint32_t>(now);
    snprintf(response, sizeof(response), "{\"timestamp\": %lu}", truncatedTimestamp);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Sending Control Unit timestamp as uint32_t: %lu", truncatedTimestamp);
    return ESP_OK;
}