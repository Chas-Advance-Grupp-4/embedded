/**
 * @file SensorUnitLinkSyncer.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the periodic status polling system.
 *
 * This file contains the implementation of the classes declared in
 * SensorUnitLinkSyncer.h:
 *
 * - `SensorUnitLinkSyncer` sets up and coordinates the task and timer trigger.
 * - `SensorUnitLinkSyncTask` defines the FreeRTOS task that collects commands
 * from backend via HTTP and adds/removes Sensor Units from manager.
 * - `SensorUnitLinkSyncTrigger` manages the periodic timer that notifies the
 * task.
 *
 * The system is designed to periodically send POSTs to a backend endpoint
 * to get commands for connect/disconnect for sensor units.
 * If a command is received it updates the SensorUnitManager accordingly
 *
 * Usage typically involves instantiating a `SensorUnitLinkSyncer`, calling
 * `start()`, and optionally `stop()`
 *
 * @date 2025-10-28
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "SensorUnitLinkSyncer.h"
#include "JsonParser.h"

SensorUnitLinkSyncer::SensorUnitLinkSyncer(RestClient&        client,
                                           SensorUnitManager& sensorUnitManager,
                                           uint64_t           intervalUs,
                                           const std::string& controlUnitId)
    : m_client(client), m_sensorUnitManager(sensorUnitManager),
      m_interval(intervalUs), m_controlUnitId(controlUnitId) {}

esp_err_t SensorUnitLinkSyncer::start() {
    m_task = std::make_unique<SensorUnitLinkSyncTask>(
        m_client, m_sensorUnitManager, m_controlUnitId);
    m_task->start();

    TaskHandle_t handle = m_task->getHandle();
    m_trigger = std::make_unique<SensorUnitLinkSyncTrigger>(handle, m_interval);
    return m_trigger->start();
}

void SensorUnitLinkSyncer::stop() {
    if (m_trigger)
        m_trigger->stop();
}

SensorUnitLinkSyncTask::SensorUnitLinkSyncTask(
    RestClient&        client,
    SensorUnitManager& sensorUnitManager,
    std::string        controlUnitId)
    : m_httpClient(client), m_sensorUnitManager(sensorUnitManager),
      m_controlUnitId(controlUnitId), m_taskHandle(nullptr) {}

void SensorUnitLinkSyncTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(
        taskEntry, "SensorUnitLinkSyncTask", 8192, this, 5, &m_taskHandle);
    ESP_LOGI(TAG, "Task created, handle: %p", m_taskHandle);
}

TaskHandle_t SensorUnitLinkSyncTask::getHandle() const {
    return m_taskHandle;
}

void SensorUnitLinkSyncTask::taskEntry(void* pvParameters) {
    auto* self = static_cast<SensorUnitLinkSyncTask*>(pvParameters);
    self->run();
}

void SensorUnitLinkSyncTask::run() {
    ESP_LOGI(TAG, "SensorUnitLinkSyncTask is running");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        ESP_LOGI(TAG, "Performing Status Polling");

        std::string json = JsonParser::composeStatusRequest(m_controlUnitId);
        RestClientResponse response =
            m_httpClient.postTo("/api/v1/control-unit/status", json);

        if (response.err != ESP_OK) {
            ESP_LOGE(TAG, "Error posting to /status");
            continue;
        }
        std::vector<SensorConnectRequest> requests =
            JsonParser::parseStatusResponse(response.payload);

        if (requests.empty()) {
            ESP_LOGE(TAG, "Invalid json response from /status");
            continue;
        }
        for (auto request : requests) {
            Uuid sensorUnitId = *request.sensorUuid;

            if (request.request == requestType::CONNECT) {
                if (m_sensorUnitManager.hasUnit(sensorUnitId)) {
                    ESP_LOGW(TAG,
                             "Sensor Unit %s already connected",
                             sensorUnitId.toString().c_str());
                } else {
                    ESP_LOGI(TAG,
                             "Connecting Sensor Unit %s",
                             sensorUnitId.toString().c_str());
                    m_sensorUnitManager.addUnit(sensorUnitId);
                }
            }

            if (request.request == requestType::DISCONNECT) {
                if (!m_sensorUnitManager.hasUnit(sensorUnitId)) {
                    ESP_LOGW(TAG,
                             "No Sensor Unit %s connected",
                             sensorUnitId.toString().c_str());
                } else {
                    ESP_LOGI(TAG,
                             "Disconnecting Sensor Unit %s",
                             sensorUnitId.toString().c_str());
                    m_sensorUnitManager.removeUnit(sensorUnitId);
                }
            }
        }
    }
}

SensorUnitLinkSyncTrigger::SensorUnitLinkSyncTrigger(TaskHandle_t targetTask,
                                                     uint64_t     intervalUs)
    : m_taskHandle(targetTask), m_interval(intervalUs), m_timer(nullptr) {}

esp_err_t SensorUnitLinkSyncTrigger::start() {
    if (m_taskHandle == nullptr) {
        ESP_LOGE(TAG, "Invalid task handle — trigger not started");
        return ESP_ERR_INVALID_ARG;
    }

    esp_timer_create_args_t timer_args{};
    timer_args.callback        = &SensorUnitLinkSyncTrigger::timerCallback;
    timer_args.arg             = this;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.name            = "SensorUnitLinkSyncTrigger";

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
             "SensorUnitLinkSyncTrigger started with interval %llu us",
             m_interval);
    return ESP_OK;
}

void SensorUnitLinkSyncTrigger::stop() {
    if (m_timer) {
        esp_timer_stop(m_timer);
        esp_timer_delete(m_timer);
        m_timer = nullptr;
        ESP_LOGI(TAG, "SensorUnitLinkSyncTrigger stopped");
    }
}

void SensorUnitLinkSyncTrigger::timerCallback(void* arg) {
    ESP_LOGI(TAG, "Timer callback triggered");
    auto* self = static_cast<SensorUnitLinkSyncTrigger*>(arg);
    ESP_LOGI(TAG,
             "Timer callback: self=%p, taskHandle=%p",
             self,
             self->m_taskHandle);
    if (self->m_taskHandle) {
        xTaskNotifyGive(self->m_taskHandle);
    }
}