/**
 * @file MockDataGenerator.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for creating a task that generates MockData 
 * simulating Sensor readings from a Sensor Unit 
 * @date 2025-10-03
 *
 * @copyright Copyright (c) 2025
 * * @license MIT
 */
#pragma once
#include "ControlUnitManager.h"
#include "RestClient.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>

/**
 * @brief Trigger for Generating Mock Values
 *
 */
class MockDataGeneratorTrigger {
  public:
    MockDataGeneratorTrigger(TaskHandle_t target_task, uint64_t interval_us);
    esp_err_t start();
    void      stop();

  private:
    static void timerCallback(void* arg);

    TaskHandle_t       m_taskHandle;
    uint64_t           m_interval;
    esp_timer_handle_t m_timer;

    static constexpr const char* TAG = "MockDataGeneratorTrigger";
};

/**
 * @brief Task that Generates Mock Values and add them to ControlUnitManager
 *
 */
class MockDataGeneratorTask {
  public:
    explicit MockDataGeneratorTask(ControlUnitManager& manager);
    void         start();
    TaskHandle_t getHandle() const;

  private:
    static void taskEntry(void* pvParameters);
    void        run();

    ControlUnitManager& m_manager;
    TaskHandle_t        m_taskHandle;

    static constexpr const char* TAG = "MockDataGeneratorTask";
};

class MockDataGenerator {
  public:
    MockDataGenerator(ControlUnitManager& manager, uint64_t interval_us);
    esp_err_t start();
    void      stop();

  private:
    ControlUnitManager&                     m_manager;
    std::unique_ptr<MockDataGeneratorTask>  m_task;
    std::unique_ptr<MockDataGeneratorTrigger> m_trigger;
    uint64_t                                m_interval;
};