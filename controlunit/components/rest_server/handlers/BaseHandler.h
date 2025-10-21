/**
 * @file BaseHandler.h
 * @brief Abstract base class for HTTP route handlers used by RestServer.
 *
 * Defines a common interface for registering URI handlers with the ESP-IDF HTTP
 * server. Each subclass must implement `getUri()` to provide its own route
 * configuration.
 *
 * Used by RestServer to dynamically register endpoints.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once

#include "esp_http_server.h"

/**
 * @class BaseHandler
 * @brief Abstract interface for HTTP route handlers.
 *
 * Provides a virtual method for returning a pointer to an `httpd_uri_t`
 * structure, which defines the URI, method, and callback function for a
 * specific HTTP endpoint.
 *
 * Subclasses must implement `getUri()` to register themselves with the server.
 */
class BaseHandler {
  public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived handler classes.
     */
    virtual ~BaseHandler() = default;
    /**
     * @brief Returns a pointer to the URI configuration for this handler.
     *
     * Must be implemented by subclasses to define the route, method, and
     * callback.
     *
     * @return Pointer to an `httpd_uri_t` structure.
     */
    virtual httpd_uri_t* getUri() = 0;
};