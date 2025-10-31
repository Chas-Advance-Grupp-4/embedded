/**
 * @file RestClient.h
 * @author Erik Dahl (erik@iunderlandet.se)
 *
 * @file RestClient.h
 * @brief Declaration of the RestClient class for HTTPS communication
 *
 * Provides a simple wrapper around the ESP-IDF HTTPS client for sending
 * JSON payloads to a remote server using HTTPS POST
 *
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include <string>

struct RestClientResponse {
  esp_err_t err;
  std::string payload;
};

/**
 * @class RestClient
 * @brief Lightweight HTTPS client for posting JSON data to a remote endpoint.
 *
 * This class wraps the ESP-IDF HTTP client and provides simplified methods
 * for initialization and sending POST requests. It supports configurable
 * base URL, JWT token (optional), and timeout settings.
 */
class RestClient {
  public:
    /**
     * @brief Constructs a RestClient with base URL, JWT token, and timeout.
     *
     * @param baseUrl Base URL of the remote server.
     * @param jwtToken JWT token for authorization
     * @param timeoutMs Timeout for HTTPS requests in milliseconds.
     */
    RestClient(const std::string& baseUrl,
               const std::string& jwtToken,
               int                timeoutMs = 5000);

    /**
     * @brief Cleans up the internal HTTPS client.
     *
     * Frees resources allocated by the ESP-IDF HTTP client.
     */
    ~RestClient();

    /**
     * @brief Initializes the internal ESP-IDF HTTP client with SSL and
     * authorization headers.
     *
     * Sets up the HTTP client using the configured base URL, timeout, and SSL
     * transport. The client is initialized with keep-alive enabled and uses the
     * default certificate bundle. Adds required headers for JWT-based
     * authorization and JSON content type.
     * 
     * It also creates/initializes the mutex used by postTo
     *
     * This method must be called before sending any HTTP requests.
     *
     * @return esp_err_t ESP_OK on successful initialization, or ESP_FAIL if the
     * client could not be created.
     */
    esp_err_t init();

    /**
     * @brief Sends a JSON payload to the specified endpoint using HTTP POST.
     *
     * Constructs the full URL by appending the endpoint to the base URL,
     * sets the request method and payload, and performs the HTTP transaction.
     * If the response body is small enough, it is read into a buffer and returned 
     * as a string. If no body is present or an error occurs during reading, 
     * the response string will be empty.
     * 
     * Mutex protected for safe calling from different tasks
     *
     * @param endpoint Relative path to the target endpoint.
     * @param payload JSON-formatted string to be sent in the request body.
     * @return RestClientResponse containing the result code, ESP_OK on success, 
     * and response body (if any).
     */
    RestClientResponse postTo(const std::string& endpoint, const std::string& payload);

  private:
  static esp_err_t httpEventHandler(esp_http_client_event_t *evt);
    std::string m_baseUrl; /**< Base URL of the remote server. */
    std::string
        m_jwtToken; /**< JWT token for authorization */
    esp_http_client_handle_t
        m_client;  /**< Handle to the ESP-IDF HTTP client. */
    int m_timeout; /**< Timeout for HTTP requests in milliseconds. */
    mutable SemaphoreHandle_t m_mutex = nullptr;
    std::string m_responseBody;
    static constexpr const char* TAG =
        "RestClient"; /**< Logging tag for ESP_LOG macros. */
};