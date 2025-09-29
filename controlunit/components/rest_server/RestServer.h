#pragma once
#include "BaseHandler.h"
#include "esp_http_server.h"
#include <memory>
#include <vector>

class RestServer {
  public:
    RestServer();
    ~RestServer();

    bool start();
    void stop();

  private:
    httpd_handle_t                            m_server;
    httpd_config_t                            m_config;
    std::vector<std::unique_ptr<BaseHandler>> m_handlers;

    void registerHandler(std::unique_ptr<BaseHandler> handler);
    void registerHandlers();
};