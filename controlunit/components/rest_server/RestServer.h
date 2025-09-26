#pragma once
#include "esp_http_server.h"

class RestServer {
public:
    RestServer();
    ~RestServer();

    bool start();
    void stop();

private:
    httpd_handle_t m_server;
    httpd_config_t m_config;

    void registerHandlers();
};