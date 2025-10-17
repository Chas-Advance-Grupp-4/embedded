/**
 * @file RestServer.h
 * @brief Declares the RestServer class for managing an embedded HTTP server.
 *
 * The RestServer class wraps the ESP-IDF HTTP server and provides a mechanism
 * for registering request handlers via the BaseHandler interface.
 *
 * It is responsible for starting and stopping the server, and for installing
 * all necessary route handlers.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "BaseHandler.h"
#include "esp_http_server.h"
#include <memory>
#include <vector>

/**
 * @class RestServer
 * @brief Manages the lifecycle and routing of an ESP-IDF HTTP server.
 *
 * Provides methods to start and stop the server, and registers handlers
 * for incoming HTTP requests using the BaseHandler abstraction.
 */
class RestServer {
  public:
    /**
     * @brief Constructs a RestServer instance.
     *
     * Initializes server configuration and prepares handler list.
     */
    RestServer();
    /**
     * @brief Destructor for RestServer.
     *
     * Cleans up server resources if still running.
     */
    ~RestServer();

    /**
     * @brief Starts the HTTP server and registers all handlers.
     * @return true if the server started successfully, false otherwise.
     */
    bool start();

    /**
     * @brief Stops the HTTP server and releases resources.
     */
    void stop();

  private:
    httpd_handle_t m_server; /**< Handle to the ESP-IDF HTTP server instance. */
    httpd_config_t m_config; /**< Configuration for the HTTP server. */
    std::vector<std::unique_ptr<BaseHandler>>
        m_handlers; /**< List of registered route handlers. */

    /**
     * @brief Registers a single HTTP handler with the server.
     * @param handler Unique pointer to a BaseHandler instance.
     */
    void registerHandler(std::unique_ptr<BaseHandler> handler);

    /**
     * @brief Registers all available HTTP handlers.
     *
     * Called internally during server startup.
     */
    void registerHandlers();
};