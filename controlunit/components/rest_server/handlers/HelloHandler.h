/**
 * @file HelloHandler.h
 * @brief Example GET handler that responds with a greeting message.
 *
 * Inherits from GetHandler and implements a simple endpoint that returns
 * a static greeting. Useful for testing server setup or providing a basic
 * health/status check.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "GetHandler.h"
#include "esp_log.h"
#include <string>

/**
 * @class HelloHandler
 * @brief Simple HTTP GET handler that returns a greeting message.
 *
 * Implements the `process()` method to respond with a predefined string.
 * Intended as a minimal example or status endpoint.
 */
class HelloHandler : public GetHandler {
  public:
    /**
     * @brief Constructs a HelloHandler for the specified URI.
     * @param uri URI path to register (e.g., "/hello").
     */
    HelloHandler(const std::string& uri);

  protected:
    /**
     * @brief Handles the incoming GET request and sends a greeting response.
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    esp_err_t process(httpd_req_t* req) override;

  private:
    std::string
        m_greeting; /**< Greeting message to be returned in the response. */
};