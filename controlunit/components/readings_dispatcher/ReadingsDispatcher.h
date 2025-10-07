/**
 * @file ReadingsDispatcher.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief High-level orchestration of periodic sensor data dispatch.
 *
 * This header defines a set of classes that work together to periodically
 * collect sensor readings and send them to a remote server.
 *
 * - `ReadingsDispatcher` is the public-facing orchestrator. It initializes and
 * connects the task and timer components.
 *
 * - `ReadingDispatchTask` is a FreeRTOS task that waits for notifications and
 * performs the actual data collection and HTTP posting.
 *
 * - `ReadingDispatchTrigger` sets up a periodic timer that notifies the task at
 * a configured interval.
 *
 * Together, these classes form a modular and reusable system for timed sensor
 * data transmission in embedded environments.
 *
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "ControlUnitManager.h"
#include "RestClient.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>

// Forward declarations for types used in ReadingsDispatcher
class ReadingDispatchTask;
class ReadingDispatchTrigger;

/**
 * @class ReadingsDispatcher
 * @brief High-level orchestrator for periodic sensor data dispatch.
 *
 * Combines a task and a timer trigger to periodically collect and send sensor
 * data. Responsible for initializing and coordinating the ReadingDispatchTask
 * and ReadingDispatchTrigger.
 */
class ReadingsDispatcher {
  public:
    /**
     * @brief Constructs a ReadingsDispatcher with client, manager, and
     * interval.
     *
     * @param client Reference to the REST client used for posting data.
     * @param manager Reference to the control unit manager providing sensor
     * data.
     * @param interval_us Timer interval in microseconds.
     */
    ReadingsDispatcher(RestClient&         client,
                       ControlUnitManager& manager,
                       uint64_t            interval_us);

    /**
     * @brief Starts the readings dispatcher by launching the task and trigger.
     *
     * Creates a ReadingDispatchTask and a ReadingDispatchTrigger, and starts
     * both.
     *
     * @return esp_err_t ESP_OK on success, or an error code on failure.
     */
    esp_err_t start();

    /**
     * @brief Stops the readings dispatcher trigger.
     *
     * Stops the periodic timer. The task is not stopped immediately.
     */
    void stop();
    // esp_err_t restart(uint64_t new_interval_us);

  private:
    RestClient&
        m_client; /**< Reference to the REST client used for posting data. */
    ControlUnitManager&
        m_manager; /**< Reference to the control unit manager. */
    std::unique_ptr<ReadingDispatchTask>
        m_task; /**< Task responsible for data dispatch. */
    std::unique_ptr<ReadingDispatchTrigger>
             m_trigger;  /**< Timer trigger for periodic task activation. */
    uint64_t m_interval; /**< Timer interval in microseconds. */
};

/**
 * @class ReadingDispatchTask
 * @brief FreeRTOS task responsible for collecting and posting sensor data.
 *
 * This task waits for notifications triggered by a timer, composes a JSON
 * payload from grouped sensor readings, and sends the data to a remote server
 * using a REST client.
 */
class ReadingDispatchTask {
  public:
    /**
     * @brief Constructs a ReadingDispatchTask with HTTP client and control unit
     * manager.
     *
     * @param client Reference to the REST client used for posting data.
     * @param manager Reference to the control unit manager providing sensor
     * data.
     */
    ReadingDispatchTask(RestClient& client, ControlUnitManager& manager);

    /**
     * @brief Starts the ReadingDispatchTask by creating a FreeRTOS task.
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
     * @brief Entry point for the ReadingDispatchTask.
     *
     * Casts the parameter to a ReadingDispatchTask instance and invokes its
     * run() method.
     *
     * @param pvParameters Pointer to the ReadingDispatchTask instance.
     */
    static void taskEntry(void* pvParameters);

    /**
     * @brief Main loop of the ReadingDispatchTask.
     *
     * Waits for task notifications, composes a JSON payload from grouped sensor
     * readings, and posts the data to the server using the REST client.
     */
    void run();

    RestClient& m_httpClient; /**< Reference to the REST client used for HTTP
                                 communication. */
    ControlUnitManager& m_manager; /**< Reference to the control unit manager
                                      providing sensor data. */
    TaskHandle_t m_taskHandle;     /**< Handle to the created FreeRTOS task. */

    static constexpr const char* TAG = "ReadingDispatchTask";
};

/**
 * @class ReadingDispatchTrigger
 * @brief Periodically triggers a task to dispatch sensor readings.
 *
 * This class sets up a periodic timer using the ESP-IDF timer API. When the
 * timer expires, it notifies a target FreeRTOS task to perform data
 * dispatching.
 */
class ReadingDispatchTrigger {
  public:
    /**
     * @brief Constructs a ReadingDispatchTrigger with a target task and
     * interval.
     *
     * @param target_task Handle to the task that will be notified periodically.
     * @param interval_us Timer interval in microseconds.
     */
    ReadingDispatchTrigger(TaskHandle_t target_task, uint64_t interval_us);

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
     * Casts the argument to a ReadingDispatchTrigger instance and sends a task
     * notification to the associated task handle, if available.
     *
     * @param arg Pointer to the ReadingDispatchTrigger instance.
     */
    static void timerCallback(void* arg);

    TaskHandle_t m_taskHandle;     /**< Handle to the task that will be notified
                                      periodically. */
    uint64_t           m_interval; /**< Timer interval in microseconds. */
    esp_timer_handle_t m_timer;    /**< Handle to the ESP-IDF timer instance. */

    static constexpr const char* TAG = "ReadingDispatchTrigger";
};