#include "RestServer.h"
#include "HelloHandler.h"
#include "PostexampleHandler.h"
#include "esp_log.h"

static const char* TAG = "RestServer";

RestServer::RestServer()
    : m_server(nullptr), m_config(HTTPD_DEFAULT_CONFIG()) {}

RestServer::~RestServer() {
    stop();
}

bool RestServer::start() {
    ESP_LOGI(TAG, "Starting REST server");
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
    registerHandler(std::make_unique<PostexampleHandler>("/postexample"));
}
