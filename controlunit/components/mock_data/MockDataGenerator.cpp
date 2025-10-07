/**
 * @file MockDataGenerator.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for creating a task that generates MockData
 * simulating Sensor readings from a Sensor Unit 
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "MockDataGenerator.h"
#include "JsonParser.h"
#include "esp_system.h"
#include <time.h>

/**
 * @brief Constructor for Mock Data Generator Trigger
 * 
 * First create a Mock Data Generator Task object if calling constructor manually
 * Or better: use MockDataGenerator class
 * 
 * @param target_task 
 * @param interval_us 
 */
MockDataGeneratorTrigger::MockDataGeneratorTrigger(TaskHandle_t target_task,
                                               uint64_t     interval_us)
    : m_taskHandle{target_task}, m_interval{interval_us}, m_timer{nullptr} {}

/**
 * @brief Initializes and starts the periodic timer for the Mock Data Generator Trigger.
 *
 * This function sets up a periodic timer using the ESP-IDF timer API. It first verifies
 * that a valid task handle is available, then configures the timer with the appropriate
 * callback and dispatch method. If successful, the timer is started with the configured
 * interval and will periodically notify the associated task.
 *
 * @return esp_err_t ESP_OK on success, or an error code indicating the failure reason.
 */

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

/**
 * @brief Stop function for Mock Data Generator Trigger
 * 
 */
void MockDataGeneratorTrigger::stop() {
    if (m_timer) {
        esp_timer_stop(m_timer);
        esp_timer_delete(m_timer);
        m_timer = nullptr;
        ESP_LOGI(TAG, "MockDataGeneratorTrigger stopped");
    }
}

/**
 * @brief Timer callback invoked by the Mock Data Generator Trigger.
 *
 * This function is called when the associated timer expires. It casts the generic
 * argument to a MockDataGeneratorTrigger instance and logs diagnostic information.
 * If a task handle is available, it sends a notification to unblock or signal the task.
 *
 * @param arg void* pointer to the MockDataGeneratorTrigger instance passed during timer setup.
 */

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


/**
 * @brief Constructor for the Mock Data Generator Task
 * 
 * @param manager This class depends on a persistent ControlUnitManager object
 */
MockDataGeneratorTask::MockDataGeneratorTask(ControlUnitManager& manager)
    : m_manager{manager}, m_taskHandle{nullptr} {}

/**
 * @brief Start function for the Mock Data Generator Task
 * 
 */
void MockDataGeneratorTask::start() {
    ESP_LOGI(TAG, "Starting task...");
    xTaskCreate(taskEntry, "MockDataGeneratorTask", 4096, this, 5, &m_taskHandle);
    ESP_LOGI(TAG, "Task created, handle: %p", m_taskHandle);
}

/**
 * @brief Get a handle to the Task
 * 
 * @return TaskHandle_t 
 */
TaskHandle_t MockDataGeneratorTask::getHandle() const {
    return m_taskHandle;
}

/**
 * @brief Entry point for the mock data generator task.
 * 
 * This function serves as the starting point for a FreeRTOS task.
 * It casts the incoming parameter to a MockDataGeneratorTask object
 * and invokes its run() method to begin execution.
 * 
 * @param pvParameters 
 */
void MockDataGeneratorTask::taskEntry(void* pvParameters) {
    auto* self = static_cast<MockDataGeneratorTask*>(pvParameters);
    self->run();
}

/**
 * @brief This is the actual Mock Data Generation function
 * 
 * It creates random readings.
 * Though quite crude it's good enough for our testing purposes
 * 
 */
void MockDataGeneratorTask::run() {
    ESP_LOGI(TAG, "MockDataGeneratorTask is running");
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ca_sensorunit_snapshot snapshot;

        snapshot.uuid =
            std::make_shared<Uuid>("987e6543-e21b-12d3-a456-426614174999");
        snapshot.timestamp = time(NULL);

        uint32_t rawTemperature = esp_random();
        snapshot.temperature    = 20.0f + (rawTemperature % 1000) / 100.0f;

        uint32_t rawHumidity = esp_random();
        snapshot.humidity    = 30 + (rawHumidity % 41);

        // Add it to the sensorManager directly instead of using the Rest Server
        m_manager.sensorManager.storeReading(snapshot);
    }
}

/**
 * @brief Constructor for the Mock Data Generator
 * 
 * This class collects the MockDataGeneratorTrigger and MockDataGeneratorTask
 * in a more convenient interface
 * 
 * @param manager The class depends on a persistent ControlUnitManager object
 * @param interval_us 
 */
MockDataGenerator::MockDataGenerator(ControlUnitManager& manager,
                                     uint64_t            interval_us)
    : m_manager{manager}, m_interval{interval_us} {}

/**
 * @brief Start function for the Mock Data Generator
 * 
 */
esp_err_t MockDataGenerator::start() {
    m_task = std::make_unique<MockDataGeneratorTask>(m_manager);
    m_task->start();

    TaskHandle_t handle = m_task->getHandle();
    m_trigger = std::make_unique<MockDataGeneratorTrigger>(handle, m_interval);
    return m_trigger->start();
}

/**
 * @brief Stop function for the Mock Data Generator
 * 
 */
void MockDataGenerator::stop() {
    if (m_trigger)
        m_trigger->stop();
}