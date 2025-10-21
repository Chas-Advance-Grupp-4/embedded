/**
 * @file GetHandler.h
 * @brief Abstract base class for handling HTTP GET requests in the REST server.
 *
 * Provides a reusable structure for defining GET endpoints using the ESP-IDF
 * HTTP server. Subclasses must implement `process()` to define the response
 * logic for incoming requests.
 *
 * Used by RestServer to register GET-based endpoints.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "BaseHandler.h"
#include "esp_http_server.h"
#include <string>

/**
 * @class GetHandler
 * @brief Abstract base class for HTTP GET route handlers.
 *
 * Inherits from BaseHandler and provides a framework for handling GET requests.
 * Subclasses must implement `process()` to define how the request is handled.
 *
 * Automatically sets up the URI and routing logic for the handler.
 */
class GetHandler : public BaseHandler {
  public:
    /**
     * @brief Returns the URI configuration for this GET handler.
     *
     * Used by RestServer to register the route with the HTTP server.
     * @return Pointer to an `httpd_uri_t` structure.
     */
    httpd_uri_t* getUri() override;

  protected:
    /**
     * @brief Constructs a GetHandler with the specified URI path.
     * @param uri URI path to register (e.g., "/status").
     */
    GetHandler(const std::string& uri);
    /**
     * @brief Processes the incoming GET request.
     *
     * Must be implemented by subclasses to define custom logic for handling the
     * request.
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    virtual esp_err_t process(httpd_req_t* req) = 0;

  private:
    std::string m_uriString; /**< Stores the URI string to ensure ownership and
                                lifetime. */
    httpd_uri_t m_uri;       /**< ESP-IDF URI configuration for this handler. */
    /**
     * @brief Static entry point for the GET handler.
     *
     * Required by ESP-IDF's C-style HTTP server, which expects a plain C
     * function pointer. This function forwards the request to the
     * instance-specific `handle()` method.
     *
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    static esp_err_t staticHandler(httpd_req_t* req);
    /**
     * @brief Internal method that invokes `process()` on the request.
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    esp_err_t handle(httpd_req_t* req);
};