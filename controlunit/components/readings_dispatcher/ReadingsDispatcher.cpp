/**
 * @file ReadingsDispatcher.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the periodic sensor data dispatch system.
 *
 * This file contains the implementation of the classes declared in ReadingsDispatcher.h:
 *
 * - `ReadingsDispatcher` sets up and coordinates the task and timer trigger.
 * - `ReadingDispatchTask` defines the FreeRTOS task that collects sensor data and sends it via HTTP.
 * - `ReadingDispatchTrigger` manages the periodic timer that notifies the task.
 *
 * The system is designed to periodically gather grouped sensor readings from the ControlUnitManager,
 * format them into JSON, and transmit them to a remote endpoint using the RestClient.
 *
 * Usage typically involves instantiating a `ReadingsDispatcher`, calling `start()`, and optionally `stop()`
 * 
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "ReadingsDispatcher.h"
#include "JsonParser.h"

/**
 * @brief Constructs a ReadingsDispatcher with client, manager, and interval.
 *
 * @param client Reference to the REST client used for posting data.
 * @param manager Reference to the control unit manager providing sensor data.
 * @param interval_us Timer interval in microseconds.
 */
ReadingsDispatcher::ReadingsDispatcher(RestClient&         client,
                                       ControlUnitManager& manager,
                                       uint64_t            interval_us)
    : m_client{client}, m_manager{manager}, m_interval{interval_us} {}

/**
 * @brief Starts the readings dispatcher by launching the task and trigger.
 *
 * Creates a ReadingDispatchTask and a ReadingDispatchTrigger, and starts both.
 *
 * @return esp_err_t ESP_OK on success, or an error code on failure.
 */
esp_err_t ReadingsDispatcher::start() {
    m_task = std::make_unique<ReadingDispatchTask>(m_client, m_manager);
    m_task->start();

    TaskHandle_t handle = m_task->getHandle();
    m_trigger = std::make_unique<ReadingDispatchTrigger>(handle, m_interval);
    return m_trigger->start();
}

/**
 * @brief Stops the readings dispatcher trigger.
 *
 * Stops the periodic timer. The task is not stopped immediately.
 */
void ReadingsDispatcher::stop() {
    if (m_trigger)
        m_trigger->stop();
    // Task not stopped immediately. Can be added if necessary
}

// ! restart not yet implemented
// esp_err_t ReadingsDispatcher::restart(uint64_t new_interval_us) {
//     if (!m_trigger) return ESP_ERR_INVALID_STATE;
//     return m_trigger->restart(new_interval_us);
// }


/**
 * @brief Constructs a ReadingDispatchTask with HTTP client and control unit manager.
 *
 * @param client Reference to the REST client used for posting data.
 * @param manager Reference to the control unit manager providing sensor data.
 */
ReadingDispatchTask::ReadingDispatchTask(RestClient&         client,
                                         ControlUnitManager& manager)
    : m_httpClient{client}, m_manager{manager}, m_taskHandle{nullptr} {}

/**
 * @brief Starts the ReadingDispatchTask by creating a FreeRTOS task.
 *
 * Creates a task with the specified name and priority, and stores its handle.
 */
void ReadingDispatchTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(taskEntry, "ReadingDispatchTask", 4096, this, 5, &m_taskHandle);
    ESP_LOGI(TAG, "Task created, handle: %p", m_taskHandle);
}

/**
 * @brief Returns the handle of the created task.
 *
 * @return TaskHandle_t Handle to the FreeRTOS task.
 */
TaskHandle_t ReadingDispatchTask::getHandle() const {
    return m_taskHandle;
}

/**
 * @brief Entry point for the ReadingDispatchTask.
 *
 * Casts the parameter to a ReadingDispatchTask instance and invokes its run() method.
 *
 * @param pvParameters Pointer to the ReadingDispatchTask instance.
 */
void ReadingDispatchTask::taskEntry(void* pvParameters) {
    auto* self = static_cast<ReadingDispatchTask*>(pvParameters);
    self->run();
}

/**
 * @brief Main loop of the ReadingDispatchTask.
 *
 * Waits for task notifications, composes a JSON payload from grouped sensor readings,
 * and posts the data to the server using the REST client.
 */
void ReadingDispatchTask::run() {
    ESP_LOGI(TAG, "ReadingDispatchTask is running");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        std::string json = JsonParser::composeGroupedReadings(
            m_manager.sensorManager.getGroupedReadings(),
            m_manager.getControlunitUuidString());
        m_httpClient.postTo("/post", json);
    }
}


/**
 * @brief Constructs a ReadingDispatchTrigger with a target task and interval.
 *
 * @param target_task Handle to the task that will be notified periodically.
 * @param interval_us Timer interval in microseconds.
 */
ReadingDispatchTrigger::ReadingDispatchTrigger(TaskHandle_t target_task,
                                               uint64_t     interval_us)
    : m_taskHandle{target_task}, m_interval{interval_us}, m_timer{nullptr} {}

/**
 * @brief Initializes and starts the periodic timer for dispatching readings.
 *
 * Verifies that a valid task handle is available, creates a timer with the specified
 * interval, and starts it. The timer will notify the target task on each tick.
 *
 * @return esp_err_t ESP_OK on success, or an error code on failure.
 */
esp_err_t ReadingDispatchTrigger::start() {
    if (m_taskHandle == nullptr) {
        ESP_LOGE(TAG, "Invalid task handle — trigger not started");
        return ESP_ERR_INVALID_ARG;
    }

    esp_timer_create_args_t timer_args{};
    timer_args.callback        = &ReadingDispatchTrigger::timerCallback;
    timer_args.arg             = this;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.name            = "ReadingDispatchTrigger";

    esp_err_t err = esp_timer_create(&timer_args, &m_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create timer: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_timer_start_periodic(m_timer, m_interval);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start timer: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG,
             "ReadingDispatchTrigger started with interval %llu us",
             m_interval);
    return ESP_OK;
}

/**
 * @brief Stops and deletes the active timer.
 *
 * If the timer is running, it is stopped and deleted. The internal timer handle
 * is reset to nullptr.
 */
void ReadingDispatchTrigger::stop() {
    if (m_timer) {
        esp_timer_stop(m_timer);
        esp_timer_delete(m_timer);
        m_timer = nullptr;
        ESP_LOGI(TAG, "ReadingDispatchTrigger stopped");
    }
}

/**
 * @brief Callback function triggered by the periodic timer.
 *
 * Casts the argument to a ReadingDispatchTrigger instance and sends a task notification
 * to the associated task handle, if available.
 *
 * @param arg Pointer to the ReadingDispatchTrigger instance.
 */
void ReadingDispatchTrigger::timerCallback(void* arg) {
    ESP_LOGI(TAG, "Timer callback triggered");
    auto* self = static_cast<ReadingDispatchTrigger*>(arg);
    ESP_LOGI(TAG,
             "Timer callback: self=%p, taskHandle=%p",
             self,
             self->m_taskHandle);
    if (self->m_taskHandle) {
        xTaskNotifyGive(self->m_taskHandle);
    }
}