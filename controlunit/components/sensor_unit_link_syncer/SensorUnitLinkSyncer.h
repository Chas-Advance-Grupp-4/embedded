/**
 * @file SensorUnitLinkSyncer.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief High-level orchestration of periodic status polling.
 *
 * This header defines a set of classes that work together to periodically
 * poll status from remote server and update SensorUnitManager.
 *
 * - `SensorUnitLinkSyncer` is the public-facing orchestrator. It initializes
 * and connects the task and timer components.
 *
 * - `SensorUnitLinkSyncTask` is a FreeRTOS task that waits for notifications
 * and performs the actual HTTP posting and updating of state.
 *
 * - `SensorUnitLinkSyncTrigger` sets up a periodic timer that notifies the task
 * at a configured interval.
 *
 *
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "RestClient.h"
#include "SensorUnitManager.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>

// Forward declarations for types used in SensorUnitLinkSyncer
class SensorUnitLinkSyncTask;
class SensorUnitLinkSyncTrigger;

/**
 * @class SensorUnitLinkSyncer
 * @brief High-level orchestrator for periodic sensor data dispatch.
 *
 * Combines a task and a timer trigger to periodically collect and send sensor
 * data. Responsible for initializing and coordinating the
 * SensorUnitLinkSyncTask and SensorUnitLinkSyncTrigger.
 */
class SensorUnitLinkSyncer {
  public:
    /**
     * @brief Constructs a SensorUnitLinkSyncer with client, sensorUnitManager,
     * and interval.
     *
     * @param client Reference to the REST client used for posting data.
     * @param sensorUnitManager Reference to the control unit manager providing
     * sensor data.
     * @param intervalUs Timer interval in microseconds.
     * @param controlUnitId The Control Unit Uuid of this particular unit.
     * Included in the HTTP POST
     */
    SensorUnitLinkSyncer(RestClient&        client,
                         SensorUnitManager& sensorUnitManager,
                         uint64_t           intervalUs,
                         const std::string& controlUnitId);

    /**
     * @brief Starts the Sensor Unit Link Syncer by launching the task and
     * trigger.
     *
     * Creates a SensorUnitLinkSyncTask and a SensorUnitLinkSyncTrigger, and
     * starts both.
     *
     * @return esp_err_t ESP_OK on success, or an error code on failure.
     */
    esp_err_t start();

    /**
     * @brief Stops the Sensor Unit Link Syncer trigger.
     *
     * Stops the periodic timer. The task is not stopped immediately.
     */
    void stop();

  private:
    RestClient&
        m_client; /**< Reference to the REST client used for posting data. */
    SensorUnitManager&
        m_sensorUnitManager; /**< Reference to the sensor unit manager. */
    std::unique_ptr<SensorUnitLinkSyncTask>
        m_task; /**< Task responsible for data dispatch. */
    std::unique_ptr<SensorUnitLinkSyncTrigger>
                m_trigger;  /**< Timer trigger for periodic task activation. */
    uint64_t    m_interval; /**< Timer interval in microseconds. */
    std::string m_controlUnitId;
};

/**
 * @class SensorUnitLinkSyncTask
 * @brief FreeRTOS task responsible for polling by posting data and based on the
 * response updating internal state.
 *
 * This task waits for notifications triggered by a timer, composes a JSON
 * payload with control unit ID sends the data to a remote server using a REST
 * client and parses the response and updates the SensorUnitManager accordingly
 * if a Sensor Unit status has changed
 *
 */
class SensorUnitLinkSyncTask {
  public:
    /**
     * @brief Constructs a SensorUnitLinkSyncTask with HTTP client and control
     * unit manager.
     *
     * @param client Reference to the REST client used for posting data.
     * @param sensorUnitManager Reference to the sensor unit manager providing
     * sensor
     * @param controlUnitId Uuid of the ControlUnit. Important to match with
     * unit_id in JWT token
     */
    SensorUnitLinkSyncTask(RestClient&        client,
                           SensorUnitManager& sensorUnitManager,
                           std::string        controlUnitId);

    /**
     * @brief Starts the SensorUnitLinkSyncTask by creating a FreeRTOS task.
     *
     * Creates a task with the specified name and priority, and stores its
     * handle.
     */
    void start();

    /**
     * @brief Returns the handle of the created task.
     *
     * @return TaskHandle_t Handle to the FreeRTOS task.
     */
    TaskHandle_t getHandle() const;

  private:
    /**
     * @brief Entry point for the SensorUnitLinkSyncTask.
     *
     * Casts the parameter to a SensorUnitLinkSyncTask instance and invokes its
     * run() method.
     *
     * @param pvParameters Pointer to the SensorUnitLinkSyncTask instance.
     */
    static void taskEntry(void* pvParameters);

    /**
     * @brief Main loop of the SensorUnitLinkSyncTask.
     *
     * Waits for task notifications, composes a JSON payload with control unit
     * id, posts the data to the server using the REST client and reads the
     * response and updates SensorUnitManager state
     */
    void run();

    RestClient& m_httpClient; /**< Reference to the REST client used for HTTP
                                 communication. */
    SensorUnitManager& m_sensorUnitManager; /**< Reference to the sensor unit
                                      manager providing sensor data. */
    std::string  m_controlUnitId;
    TaskHandle_t m_taskHandle; /**< Handle to the created FreeRTOS task. */

    static constexpr const char* TAG = "SensorUnitLinkSyncTask";
};

/**
 * @class SensorUnitLinkSyncTrigger
 * @brief Periodically triggers a task to dispatch sensor readings.
 *
 * This class sets up a periodic timer using the ESP-IDF timer API. When the
 * timer expires, it notifies a target FreeRTOS task to perform data
 * dispatching.
 */
class SensorUnitLinkSyncTrigger {
  public:
    /**
     * @brief Constructs a SensorUnitLinkSyncTrigger with a target task and
     * interval.
     *
     * @param target_task Handle to the task that will be notified periodically.
     * @param interval_us Timer interval in microseconds.
     */
    SensorUnitLinkSyncTrigger(TaskHandle_t targetTask, uint64_t intervalUs);

    /**
     * @brief Initializes and starts the periodic timer for dispatching
     * readings.
     *
     * Verifies that a valid task handle is available, creates a timer with the
     * specified interval, and starts it. The timer will notify the target task
     * on each tick.
     *
     * @return esp_err_t ESP_OK on success, or an error code on failure.
     */
    esp_err_t start();

    /**
     * @brief Stops and deletes the active timer.
     *
     * If the timer is running, it is stopped and deleted. The internal timer
     * handle is reset to nullptr.
     */
    void stop();

  private:
    /**
     * @brief Callback function triggered by the periodic timer.
     *
     * Casts the argument to a SensorUnitLinkSyncTrigger instance and sends a
     * task notification to the associated task handle, if available.
     *
     * @param arg Pointer to the SensorUnitLinkSyncTrigger instance.
     */
    static void timerCallback(void* arg);

    TaskHandle_t m_taskHandle;     /**< Handle to the task that will be notified
                                      periodically. */
    uint64_t           m_interval; /**< Timer interval in microseconds. */
    esp_timer_handle_t m_timer;    /**< Handle to the ESP-IDF timer instance. */

    static constexpr const char* TAG = "SensorUnitLinkSyncTrigger";
};