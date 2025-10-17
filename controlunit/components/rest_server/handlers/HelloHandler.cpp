/**
 * @file HelloHandler.cpp
 * @brief Implementation of HelloHandler, a simple GET endpoint that returns a
 * greeting.
 *
 * Defines the logic for responding to HTTP GET requests with a static greeting
 * message. Inherits from GetHandler and overrides the `process()` method to
 * send a plain-text response.
 *
 * Useful for testing server availability or providing a basic health check.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "HelloHandler.h"

HelloHandler::HelloHandler(const std::string& uri)
    : GetHandler(uri), m_greeting("Hello from ESP32 with generic GetHandler") {}

esp_err_t HelloHandler::process(httpd_req_t* req) {
    httpd_resp_set_type(req, "text/plain; charset=utf-8");
    httpd_resp_send(req, m_greeting.c_str(), m_greeting.length());
    return ESP_OK;
}