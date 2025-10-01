#pragma once

#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ReadingDispatchTrigger
class SensorTrigger {
public:
    SensorTrigger(TaskHandle_t target_task, uint64_t interval_us)
        : task_handle(target_task), interval(interval_us), timer(nullptr) {}

    esp_err_t start() {
        esp_timer_create_args_t timer_args = {
            .callback = &SensorTrigger::timer_callback,
            .arg = this,
            .name = "SensorTrigger"
        };

        esp_err_t err = esp_timer_create(&timer_args, &timer);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Kunde inte skapa timer: %s", esp_err_to_name(err));
            return err;
        }

        err = esp_timer_start_periodic(timer, interval);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Kunde inte starta timer: %s", esp_err_to_name(err));
            return err;
        }

        ESP_LOGI(TAG, "SensorTrigger startad");
        return ESP_OK;
    }

    void stop() {
        if (timer) {
            esp_timer_stop(timer);
            esp_timer_delete(timer);
            timer = nullptr;
            ESP_LOGI(TAG, "SensorTrigger stoppad");
        }
    }

private:
    static void timer_callback(void* arg) {
        auto* self = static_cast<SensorTrigger*>(arg);
        if (self->task_handle) {
            xTaskNotifyGive(self->task_handle);
        }
    }

    TaskHandle_t task_handle;
    uint64_t interval;
    esp_timer_handle_t timer;

    static constexpr const char* TAG = "SensorTrigger";
};