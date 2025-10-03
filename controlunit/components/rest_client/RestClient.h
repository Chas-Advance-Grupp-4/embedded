#pragma once

#include "esp_http_client.h"
#include "esp_log.h"
#include <string>

// RestClient
class RestClient {
  public:
    RestClient(const std::string& base_url,
               const std::string& jwt_token,
               int                timeout_ms = 5000);
    ~RestClient();

    esp_err_t init();
    esp_err_t postTo(const std::string& endpoint, const std::string& payload);

  private:
    std::string                  m_baseUrl;
    std::string                  m_jwtToken; // unused for now
    esp_http_client_handle_t     m_client;
    int                          m_timeout;
    static constexpr const char* TAG = "RestClient";
};