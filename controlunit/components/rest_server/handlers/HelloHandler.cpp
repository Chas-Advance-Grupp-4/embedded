#include "HelloHandler.h"

HelloHandler::HelloHandler(const std::string& uri)
    : GetHandler(uri), m_greeting("Hello from ESP32 with generic GetHandler") {}

esp_err_t HelloHandler::process(httpd_req_t* req) {
    httpd_resp_set_type(req, "text/plain; charset=utf-8");
    httpd_resp_send(req, m_greeting.c_str(), m_greeting.length());
    return ESP_OK;
}