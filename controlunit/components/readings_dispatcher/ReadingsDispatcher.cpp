#include "ReadingsDispatcher.h"

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

ReadingDispatchTask::ReadingDispatchTask(RestClient&         client,
                                         ControlUnitManager& manager)
    : m_httpClient{client}, m_manager{manager}, m_taskHandle{nullptr} {}

void ReadingDispatchTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(taskEntry, "ReadingDispatchTask", 4096, this, 5, &m_taskHandle);
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
        // add m_manager.buildJson();  eller vad den nu heter
        std::string json =
            "{\"content\":\"Hello with ControlUnitManager in the backseat\"}";
        m_httpClient.postTo("/post", json);
    }
}

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

// ! restart not yet implemented
// esp_err_t ReadingsDispatcher::restart(uint64_t new_interval_us) {
//     if (!m_trigger) return ESP_ERR_INVALID_STATE;
//     return m_trigger->restart(new_interval_us);
// }