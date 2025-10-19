#pragma once
#include "PostHandler.h"
#include "SensorUnitManager.h"

/**
 * @class ConnectHandler
 * @brief Handles HTTP POST requests for sensor unit connection status.
 *
 * This handler processes incoming POST requests containing sensor unit identifiers
 * and determines their connection status based on the SensorUnitManager.
 * It responds with a JSON payload indicating whether the unit is connected,
 * pending, or invalid.
 *
 * Inherits from PostHandler and overrides the processBody method to implement
 * custom logic for sensor unit connection handling.
 */
class ConnectHandler : public PostHandler {
public:
    ConnectHandler(const std::string& uri, SensorUnitManager& sensorUnitManager);

protected:
    esp_err_t processBody(httpd_req_t* req, const std::string& body) override;

private: 
    const SensorUnitManager& m_sensorUnitManager;
    static constexpr const char* TAG = "ConnectHandler"; /**< Logging tag for ESP_LOG macros. */
};