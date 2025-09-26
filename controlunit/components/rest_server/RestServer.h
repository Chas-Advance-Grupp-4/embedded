#pragma once
#include "esp_http_server.h"
#include "BaseHandler.h"
#include <vector>
#include <memory>

class RestServer {
public:
    RestServer();
    ~RestServer();

    bool start();
    void stop();

private:
    httpd_handle_t m_server;
    httpd_config_t m_config;
    std::vector<std::unique_ptr<BaseHandler>> m_handlers;

    void registerHandlers();
    void registerHandler(std::unique_ptr<BaseHandler> handler);
};