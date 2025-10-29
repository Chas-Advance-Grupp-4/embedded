/**
 * @file ReadingsDispatcher.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the periodic sensor data dispatch system.
 *
 * This file contains the implementation of the classes declared in
 * ReadingsDispatcher.h:
 *
 * - `ReadingsDispatcher` sets up and coordinates the task and timer trigger.
 * - `ReadingDispatchTask` defines the FreeRTOS task that collects sensor data
 * and sends it via HTTP.
 * - `ReadingDispatchTrigger` manages the periodic timer that notifies the task.
 *
 * The system is designed to periodically gather grouped sensor readings from
 * the ControlUnitManager, format them into JSON, and transmit them to a remote
 * endpoint using the RestClient.
 *
 * Usage typically involves instantiating a `ReadingsDispatcher`, calling
 * `start()`, and optionally `stop()`
 *
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "ReadingsDispatcher.h"
#include "JsonParser.h"

ReadingsDispatcher::ReadingsDispatcher(RestClient&         client,
                                       ControlUnitManager& manager,
                                       uint64_t            interval_us)
    : m_client{client}, m_manager{manager}, m_interval{interval_us} {}

esp_err_t ReadingsDispatcher::start() {
    m_task = std::make_unique<ReadingDispatchTask>(m_client, m_manager);
    m_task->start();

    TaskHandle_t handle = m_task->getHandle();
    m_trigger = std::make_unique<ReadingDispatchTrigger>(handle, m_interval);
    return m_trigger->start();
}

void ReadingsDispatcher::stop() {
    if (m_trigger)
        m_trigger->stop();
    // Task not stopped immediately. Can be added if necessary
}

ReadingDispatchTask::ReadingDispatchTask(RestClient&         client,
                                         ControlUnitManager& manager)
    : m_httpClient{client}, m_manager{manager}, m_taskHandle{nullptr} {}

void ReadingDispatchTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(taskEntry, "ReadingDispatchTask", 8192, this, 5, &m_taskHandle);
    ESP_LOGI(TAG, "Task created, handle: %p", m_taskHandle);
}

TaskHandle_t ReadingDispatchTask::getHandle() const {
    return m_taskHandle;
}

void ReadingDispatchTask::taskEntry(void* pvParameters) {
    auto* self = static_cast<ReadingDispatchTask*>(pvParameters);
    self->run();
}

void ReadingDispatchTask::run() {
    ESP_LOGI(TAG, "ReadingDispatchTask is running");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        std::string json = JsonParser::composeGroupedReadings(
            m_manager.sensorManager.getGroupedReadings(),
            m_manager.getControlunitUuidString());
        RestClientResponse response =
            m_httpClient.postTo("/api/v1/control-unit", json);
        if (response.err != ESP_OK) {
            ESP_LOGW(TAG, "POST to /api/v1/control-unit failed");
        } else {
            size_t savedReadings =
                JsonParser::parseBackendReadingsResponse(response.payload);
            if (savedReadings == 0) {
                ESP_LOGW(TAG, "Successful posting but saved readings 0");
            } else {
                ESP_LOGI(
                    TAG,
                    "Successful posting. Clearing %zu readings from buffer",
                    savedReadings);
                m_manager.sensorManager.clearReadings(savedReadings);
            }
        }
    }
}

ReadingDispatchTrigger::ReadingDispatchTrigger(TaskHandle_t target_task,
                                               uint64_t     interval_us)
    : m_taskHandle{target_task}, m_interval{interval_us}, m_timer{nullptr} {}

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

void ReadingDispatchTrigger::stop() {
    if (m_timer) {
        esp_timer_stop(m_timer);
        esp_timer_delete(m_timer);
        m_timer = nullptr;
        ESP_LOGI(TAG, "ReadingDispatchTrigger stopped");
    }
}

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