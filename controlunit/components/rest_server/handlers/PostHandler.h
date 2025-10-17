/**
 * @file PostHandler.h
 * @brief Abstract base class for handling HTTP POST requests in the REST
 * server.
 *
 * Provides a reusable structure for defining POST endpoints using the ESP-IDF
 * HTTP server. Subclasses must implement `processBody()` to handle incoming
 * request payloads.
 *
 * Used by RestServer to register and route POST-based endpoints.
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
 * @class PostHandler
 * @brief Abstract base class for HTTP POST route handlers.
 *
 * Inherits from BaseHandler and provides a framework for handling POST
 * requests. Subclasses must implement `processBody()` to define how the request
 * body is processed.
 *
 * Automatically sets up the URI and routing logic for the handler.
 */
class PostHandler : public BaseHandler {
  public:
    /**
     * @brief Returns the URI configuration for this POST handler.
     *
     * Used by RestServer to register the route with the HTTP server.
     * @return Pointer to an `httpd_uri_t` structure.
     */
    httpd_uri_t* getUri() override;

  protected:
    /**
     * @brief Constructs a PostHandler with the specified URI path.
     * @param uri URI path to register (e.g., "/post").
     */
    PostHandler(const std::string& uri);

    /**
     * @brief Processes the body of the incoming POST request.
     *
     * Must be implemented by subclasses to define custom logic for handling the
     * payload.
     * @param req Pointer to the HTTP request object.
     * @param body Parsed request body as a string.
     * @return ESP_OK on success, or an appropriate error code.
     */
    virtual esp_err_t processBody(httpd_req_t*       req,
                                  const std::string& body) = 0;

  private:
    std::string m_uriString; /**< Stores the URI string to ensure ownership and
                                lifetime. */
    httpd_uri_t m_uri;       /**< ESP-IDF URI configuration for this handler. */
    /**
     * @brief Static entry point for the POST handler.
     *
     * Forwards the request to the instance-specific `handle()` method.
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    static esp_err_t staticHandler(httpd_req_t* req);
    /**
     * @brief Internal method that reads the request body and calls
     * `processBody()`.
     * @param req Pointer to the HTTP request object.
     * @return ESP_OK on success, or an appropriate error code.
     */
    esp_err_t handle(httpd_req_t* req);
};