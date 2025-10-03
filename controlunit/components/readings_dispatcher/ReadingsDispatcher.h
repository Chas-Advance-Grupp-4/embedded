#pragma once
#include "ControlUnitManager.h"
#include "RestClient.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>

class ReadingDispatchTrigger {
  public:
    ReadingDispatchTrigger(TaskHandle_t target_task, uint64_t interval_us);
    esp_err_t start();
    void      stop();

  private:
    static void timerCallback(void* arg);

    TaskHandle_t       m_taskHandle;
    uint64_t           m_interval;
    esp_timer_handle_t m_timer;

    static constexpr const char* TAG = "ReadingDispatchTrigger";
};

class ReadingDispatchTask {
  public:
    ReadingDispatchTask(RestClient& client, ControlUnitManager& manager);
    void         start();
    TaskHandle_t getHandle() const;

  private:
    static void taskEntry(void* pvParameters);
    void        run();

    RestClient&         m_httpClient;
    ControlUnitManager& m_manager;
    TaskHandle_t        m_taskHandle;

    static constexpr const char* TAG = "ReadingDispatchTask";
};

class ReadingsDispatcher {
  public:
    ReadingsDispatcher(RestClient&         client,
                       ControlUnitManager& manager,
                       uint64_t            interval_us);
    esp_err_t start();
    void      stop();
    // esp_err_t restart(uint64_t new_interval_us);

  private:
    RestClient&                             m_client;
    ControlUnitManager&                     m_manager;
    std::unique_ptr<ReadingDispatchTask>    m_task;
    std::unique_ptr<ReadingDispatchTrigger> m_trigger;
    uint64_t                                m_interval;
};