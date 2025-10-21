
/**
 * @file TimeHandler.h
 * @brief GET /time handler that return JSON with a Unix timestamp 
 *
 * Has a reference to TimeSyncManager. If time is synced it sends
 * a Unix Timestamp in JSON with key "timestamp"
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-16
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "GetHandler.h"
#include "TimeSyncManager.h"
#include <string>

/**
 * @class TimeHandler
 * @brief HTTP GET /time handler that return JSON with a Unix timestamp 
 *
 */
class TimeHandler : public GetHandler {
  public:
    /**
     * @brief Constructs a TimeHandler for the specified URI.
     * @param uri URI path to register (e.g., "/time").
     */
    TimeHandler(const std::string& uri, TimeSyncManager& timeSyncManager);

  protected:
    /**
     * @brief The handler implementation that gets called by handle. 
     * GET /time - If internal time is synced it dispatches a JSON 
     * with current time as a Unix timestamp 
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, else ESP_FAIL with error description
     */
    esp_err_t process(httpd_req_t* req) override;

  private:
    TimeSyncManager& m_timeSyncManager;    
    static constexpr const char* TAG =
        "TimeHandler"; /**< Logging tag for ESP_LOG macros. */
};