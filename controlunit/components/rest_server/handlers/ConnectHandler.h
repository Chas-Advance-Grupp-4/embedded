/**
 * @file ConnectHandler.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief HTTP POST handler for processing sensor unit connection requests.
 *
 * Defines the ConnectHandler class, which handles incoming POST requests
 * containing sensor unit identifiers. It determines their connection status
 * using SensorUnitManager and responds with a JSON payload indicating
 * whether the unit is connected, pending, or invalid.
 * 
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
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
    /**
     * @brief Constructs a ConnectHandler with a specific URI and sensor unit manager.
     * @param uri The URI this handler responds to.
     * @param sensorUnitManager Reference to the sensor unit manager used for status lookup.
     */
    ConnectHandler(const std::string& uri, SensorUnitManager& sensorUnitManager);

protected:
    /**
     * @brief Processes the body of an HTTP POST request.
     *
     * Parses the incoming JSON payload to extract a sensor unit ID,
     * checks its validity and connection status, and responds with
     * a JSON-formatted status message.
     *
     * @param req Pointer to the HTTP request object.
     * @param body The raw POST body as a string.
     * @return ESP_OK on success, or ESP_FAIL if the input is invalid.
     */
    esp_err_t processBody(httpd_req_t* req, const std::string& body) override;

private: 
    const SensorUnitManager& m_sensorUnitManager;
    static constexpr const char* TAG = "ConnectHandler"; /**< Logging tag for ESP_LOG macros. */
};