/**
 * @file RestServer.cpp
 * @brief Implements the RestServer class for managing HTTP server and route
 * handlers.
 *
 * Defines the logic for starting and stopping the ESP-IDF HTTP server,
 * and for registering route handlers via the BaseHandler interface.
 *
 * Used to expose REST endpoints for units connecting on local AP
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "RestServer.h"
#include "ConnectHandler.h"
#include "HelloHandler.h"
#include "TimeHandler.h"
#include "esp_log.h"

static const char* TAG = "RestServer";

RestServer::RestServer(uint16_t           port,
                       TimeSyncManager&   timeSyncManager,
                       SensorUnitManager& sensorUnitManager)
    : m_server(nullptr), m_config(HTTPD_DEFAULT_CONFIG()), m_port(port),
      m_timeSyncManager(timeSyncManager),
      m_sensorUnitManager(sensorUnitManager) {}

RestServer::~RestServer() {
    stop();
}

bool RestServer::start() {
    ESP_LOGI(TAG, "Starting REST server");
    m_config.server_port = m_port;

    if (httpd_start(&m_server, &m_config) == ESP_OK) {
        registerHandlers();
        ESP_LOGI(TAG, "Server started");
        return true;
    }
    ESP_LOGE(TAG, "Failed to start server");
    return false;
}

void RestServer::stop() {
    if (m_server) {
        httpd_stop(m_server);
        m_server = nullptr;
        ESP_LOGI(TAG, "Server stopped");
    }
}

void RestServer::registerHandler(std::unique_ptr<BaseHandler> handler) {
    httpd_uri_t* uri    = handler->getUri();
    esp_err_t    result = httpd_register_uri_handler(m_server, uri);

    if (result != ESP_OK) {
        ESP_LOGE(
            "RestServer", "Failed to register handler for URI: %s", uri->uri);
        return;
    }

    m_handlers.push_back(std::move(handler));
    ESP_LOGI("RestServer", "Registered handler for URI: %s", uri->uri);
}

void RestServer::registerHandlers() {
    registerHandler(std::make_unique<HelloHandler>("/hello"));
    registerHandler(std::make_unique<ConnectHandler>("/connect", m_sensorUnitManager));
    registerHandler(std::make_unique<TimeHandler>("/time", m_timeSyncManager));
}
