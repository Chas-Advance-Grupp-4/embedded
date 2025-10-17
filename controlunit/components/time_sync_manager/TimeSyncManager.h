/**
 * @file TimeSyncManager.h
 * @brief Provides functionality to synchronize the ESP system clock with an NTP
 * server.
 *
 * This class handles initial time synchronization using SNTP and optionally
 * sets up a periodic task to resync the clock at regular intervals.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>

/**
 * @class TimeSyncManager
 * @brief Manages synchronization of the ESP system clock with an NTP server.
 *
 * Supports initial time sync and periodic resync using SNTP. Designed to ensure
 * the system clock remains accurate over time, even in long-running
 * deployments.
 */
class TimeSyncManager {
  public:
    /**
     * @brief Constructs a TimeSyncManager with NTP server and sync interval.
     * @param ntpServer Address of the NTP server (default: "se.pool.ntp.org").
     * @param syncIntervalMs Interval between automatic resyncs in milliseconds
     * (default: 1 hour).
     */
    TimeSyncManager(const std::string& ntpServer      = "se.pool.ntp.org",
                    uint32_t           syncIntervalMs = 60 * 60 * 1000);

    /**
     * @brief Performs initial time synchronization with the configured NTP
     * server.
     *
     * Initializes SNTP, waits for a valid timestamp, and optionally starts
     * auto-resync.
     */
    void start();
    /**
     * @brief Enables periodic resynchronization of the system clock.
     *
     * Starts a FreeRTOS task that calls resync() at the configured interval.
     */
    void enableAutoResync();
    /**
     * @brief Performs a manual resynchronization of the system clock.
     *
     * Stops and restarts SNTP, then checks if the time was successfully
     * updated.
     */
    void resync();

  private:
    std::string m_ntpServer;      /**< Address of the NTP server to use. */
    uint32_t    m_syncIntervalMs; /**< Interval between automatic resyncs in
                                     milliseconds. */
    bool m_timeSynced;    /**< Indicates whether the time has been successfully
                             synced. */
    int m_maxRetries{10}; /**< Maximum number of retries during initial sync. */
    TaskHandle_t m_resyncTaskHandle =
        nullptr; /**< Handle to the resync FreeRTOS task. */

    /**
     * @brief Internal task function used for periodic resync.
     *
     * Called by the FreeRTOS task created in enableAutoResync().
     */
    void resyncTask();

    static constexpr const char* TAG =
        "TimeSyncManager"; /**< Logging tag for ESP_LOG macros. */
};