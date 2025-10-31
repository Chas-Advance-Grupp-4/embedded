/**
 * @file TimeHandler.cpp
 * @brief Implementation of GET /time endpoint
 *
 * Has a reference to TimeSyncManager. If time is synced it sends
 * a Unix Timestamp in JSON with key "timestamp"
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-16
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "TimeHandler.h"
#include "JsonParser.h"
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
    
    std::string payload = JsonParser::composeTimestampPayload(now);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, payload.c_str(), HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Sending Control Unit timestamp as double: %.0f", static_cast<double>(now));
    return ESP_OK;
}