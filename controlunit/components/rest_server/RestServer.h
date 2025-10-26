/**
 * @file RestServer.h
 * @brief Declares the RestServer class for managing an embedded HTTPS server.
 *
 * The RestServer class wraps the ESP-IDF HTTPS server and provides a mechanism
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
#include "TimeSyncManager.h"
#include "SensorUnitManager.h"
#include "esp_https_server.h"
#include <memory>
#include <vector>

/**
 * @class RestServer
 * @brief Manages the lifecycle and routing of an ESP-IDF HTTPS server.
 *
 * Provides methods to start and stop the server, and registers handlers
 * for incoming HTTPS requests using the BaseHandler abstraction.
 */
class RestServer {
  public:
    /**
     * @brief Constructs a RestServer instance.
     *
     * Initializes server configuration and prepares handler list.
     */
    RestServer(TimeSyncManager& timeSyncManager, SensorUnitManager& sensorUnitManager, uint16_t port = 443);
    /**
     * @brief Destructor for RestServer.
     *
     * Cleans up server resources if still running.
     */
    ~RestServer();

    /**
     * @brief Starts the HTTPS server and registers all handlers.
     * @return true if the server started successfully, false otherwise.
     */
    bool start();

    /**
     * @brief Stops the HTTPS server and releases resources.
     */
    void stop();

  private:
  /**
   * @brief Registers a single HTTPS handler with the server.
   * @param handler Unique pointer to a BaseHandler instance.
   */
  void registerHandler(std::unique_ptr<BaseHandler> handler);

  /**
   * @brief Registers all available HTTPS handlers.
   *
   * Called internally during server startup.
   */
  void registerHandlers();

  httpd_handle_t m_server; /**< Handle to the ESP-IDF HTTPS server instance. */
    httpd_ssl_config_t m_config; /**< Configuration for the HTTPS server. */
    uint16_t m_port; /**< Port for the HTTPS server */
    std::vector<std::unique_ptr<BaseHandler>>
        m_handlers; /**< List of registered route handlers. */
    TimeSyncManager& m_timeSyncManager; /**< TimeSyncManager dependency used by TimeHandler */
    SensorUnitManager& m_sensorUnitManager; /**< SensorUnitManager dependency used by ConnectHandler */
};