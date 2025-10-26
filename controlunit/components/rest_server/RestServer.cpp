/**
 * @file RestServer.cpp
 * @brief Implements the RestServer class for managing HTTPS server and route
 * handlers.
 *
 * Defines the logic for starting and stopping the ESP-IDF HTTPS server,
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
#include "server_cert.h"
#include "server_key.h"

static const char* TAG = "RestServer";

RestServer::RestServer(TimeSyncManager&   timeSyncManager,
                       SensorUnitManager& sensorUnitManager,
                       uint16_t           port)
    : m_server(nullptr), m_config(HTTPD_SSL_CONFIG_DEFAULT()), m_port(port),
      m_timeSyncManager(timeSyncManager),
      m_sensorUnitManager(sensorUnitManager) {}

RestServer::~RestServer() {
    stop();
}

bool RestServer::start() {
    ESP_LOGI(TAG, "Starting REST server");
    m_config.transport_mode = HTTPD_SSL_TRANSPORT_SECURE;
    m_config.port_secure = m_port;

    extern const unsigned char servercert_start[] asm("_binary_server_cert_pem_start");
    extern const unsigned char servercert_end[]   asm("_binary_server_cert_pem_end");
    m_config.servercert = servercert_start;
    m_config.servercert_len = servercert_end - servercert_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_server_key_pem_start");
    extern const unsigned char prvtkey_pem_end[]   asm("_binary_server_key_pem_end");
    m_config.prvtkey_pem = prvtkey_pem_start;
    m_config.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    // m_config.servercert     = reinterpret_cast<const unsigned char*>(server_cert_pem_string.c_str());
    // m_config.servercert_len = server_cert_pem_string.length();

    // m_config.prvtkey_pem = server_key_pem;
    // m_config.prvtkey_len = server_key_pem_len;
    // m_config.user_cb
 
    if (httpd_ssl_start(&m_server, &m_config) == ESP_OK) {
        registerHandlers();
        ESP_LOGI(TAG, "Server started");
        return true;
    }
    ESP_LOGE(TAG, "Failed to start server");
    return false;
}

void RestServer::stop() {
    if (m_server) {
        httpd_ssl_stop(m_server);
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
    // registerHandler(std::make_unique<HelloHandler>("/hello"));
    registerHandler(
        std::make_unique<ConnectHandler>("/connect", m_sensorUnitManager));
    registerHandler(std::make_unique<TimeHandler>("/time", m_timeSyncManager));
}
