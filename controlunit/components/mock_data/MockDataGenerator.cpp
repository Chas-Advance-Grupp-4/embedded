#include "MockDataGenerator.h"
#include "JsonParser.h"
#include "esp_system.h"
#include <time.h>


MockDataGeneratorTrigger::MockDataGeneratorTrigger(TaskHandle_t target_task,
                                               uint64_t     interval_us)
    : m_taskHandle{target_task}, m_interval{interval_us}, m_timer{nullptr} {}

esp_err_t MockDataGeneratorTrigger::start() {
    if (m_taskHandle == nullptr) {
        ESP_LOGE(TAG, "Invalid task handle — trigger not started");
        return ESP_ERR_INVALID_ARG;
    }

    esp_timer_create_args_t timer_args{};
    timer_args.callback        = &MockDataGeneratorTrigger::timerCallback;
    timer_args.arg             = this;
    timer_args.dispatch_method = ESP_TIMER_TASK;
    timer_args.name            = "MockDataGeneratorTrigger";

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
             "MockDataGeneratorTrigger started with interval %llu us",
             m_interval);
    return ESP_OK;
}

void MockDataGeneratorTrigger::stop() {
    if (m_timer) {
        esp_timer_stop(m_timer);
        esp_timer_delete(m_timer);
        m_timer = nullptr;
        ESP_LOGI(TAG, "MockDataGeneratorTrigger stopped");
    }
}

void MockDataGeneratorTrigger::timerCallback(void* arg) {
    ESP_LOGI(TAG, "Timer callback triggered");
    auto* self = static_cast<MockDataGeneratorTrigger*>(arg);
    ESP_LOGI(TAG,
             "Timer callback: self=%p, taskHandle=%p",
             self,
             self->m_taskHandle);
    if (self->m_taskHandle) {
        xTaskNotifyGive(self->m_taskHandle);
    }
}



MockDataGeneratorTask::MockDataGeneratorTask(ControlUnitManager& manager)
    : m_manager{manager}, m_taskHandle{nullptr} {}

void MockDataGeneratorTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(taskEntry, "MockDataGeneratorTask", 4096, this, 5, &m_taskHandle);
    ESP_LOGI(TAG, "Task created, handle: %p", m_taskHandle);
}

TaskHandle_t MockDataGeneratorTask::getHandle() const {
    return m_taskHandle;
}

void MockDataGeneratorTask::taskEntry(void* pvParameters) {
    auto* self = static_cast<MockDataGeneratorTask*>(pvParameters);
    self->run();
}

void MockDataGeneratorTask::run() {
    ESP_LOGI(TAG, "MockDataGeneratorTask is running");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Create a random reading (not very sophisticated)
        ca_sensorunit_snapshot snapshot;

        snapshot.uuid =
            std::make_shared<Uuid>("987e6543-e21b-12d3-a456-426614174999");
        snapshot.timestamp = time(NULL);

        uint32_t rawTemperature = esp_random();
        snapshot.temperature    = 20.0f + (rawTemperature % 1000) / 100.0f;

        uint32_t rawHumidity = esp_random();
        snapshot.humidity    = 30 + (rawHumidity % 41);

        // Add it to the sensorManager directly
        m_manager.sensorManager.storeReading(snapshot);
    }
}

MockDataGenerator::MockDataGenerator(ControlUnitManager& manager,
                                     uint64_t            interval_us)
    : m_manager{manager}, m_interval{interval_us} {}

esp_err_t MockDataGenerator::start() {
    m_task = std::make_unique<MockDataGeneratorTask>(m_manager);
    m_task->start();

    TaskHandle_t handle = m_task->getHandle();
    m_trigger = std::make_unique<MockDataGeneratorTrigger>(handle, m_interval);
    return m_trigger->start();
}
void MockDataGenerator::stop() {
    if (m_trigger)
        m_trigger->stop();
}