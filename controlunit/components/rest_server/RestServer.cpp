#include "RestServer.h"
#include "esp_log.h"
#include "hello_handler.h"
#include "postexample_handler.h"
#include "status_handler.h"

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

void RestServer::registerHandlers() {
    httpd_uri_t hello_uri = {.uri      = "/hello",
                             .method   = HTTP_GET,
                             .handler  = hello_get_handler,
                             .user_ctx = nullptr};
    httpd_register_uri_handler(m_server, &hello_uri);

    httpd_uri_t status_uri = {.uri      = "/status",
                              .method   = HTTP_GET,
                              .handler  = status_get_handler,
                              .user_ctx = nullptr};
    httpd_register_uri_handler(m_server, &status_uri);

    httpd_uri_t postexample_uri = {.uri      = "/postexample",
                                   .method   = HTTP_POST,
                                   .handler  = postexample_post_handler,
                                   .user_ctx = nullptr};
    httpd_register_uri_handler(m_server, &postexample_uri);
}