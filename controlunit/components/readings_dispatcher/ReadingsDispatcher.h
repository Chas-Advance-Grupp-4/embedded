#pragma once

#include "RestClient.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class ReadingDispatchTrigger {
public:
    ReadingDispatchTrigger(TaskHandle_t target_task, uint64_t interval_us);
    esp_err_t start();    
    void stop();
    
private:
    static void timerCallback(void* arg);
    
    TaskHandle_t m_taskHandle;
    uint64_t m_interval;
    esp_timer_handle_t m_timer;

    static constexpr const char* TAG = "ReadingDispatchTrigger";
};

class ReadingDispatchTask {
public:
    ReadingDispatchTask(RestClient& client);
    void start();    
    TaskHandle_t getHandle() const;
    
private:
    static void taskEntry(void* pvParameters);    
    void run();
    
    RestClient& m_httpClient;
    TaskHandle_t m_taskHandle;

    static constexpr const char* TAG = "ReadingDispatchTask";
};