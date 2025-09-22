/**
 * @brief main file for the test_runner
 * 
 * This is for running unit tests. It is still just an example
 * For now it runs them on the hardware
 * Will be separated into native logic tests and also Dockerized later on
 * 
 */
extern "C" {
    #include <unity.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <esp_log.h>
}

// Deklarera testfunktionen från komponenten
extern "C" void test_hello_handler_returns_ok(void);
extern "C" void when_manager_empty_then_hasUnit_returns_false(void);
extern "C" void when_unit_added_then_hasUnit_returns_true(void);
extern "C" void when_unit_added_and_removed_then_hasUnit_returns_false(void);
extern "C" void when_unit_added_twice_then_logs_error(void);
extern "C" void when_nonexistent_unit_removed_then_logs_error(void);
extern "C" void stress_test_many_units(void);

// Lägg till testen i main
extern "C" void app_main() {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("TEST", "Test-runner startar...");
    UNITY_BEGIN();
    RUN_TEST(test_hello_handler_returns_ok);
    UNITY_END();

    ESP_LOGI("TEST", "Testing SensorUnitManager");
    UNITY_BEGIN();
    RUN_TEST(when_manager_empty_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_then_hasUnit_returns_true);     
    RUN_TEST(when_unit_added_and_removed_then_hasUnit_returns_false);
    RUN_TEST(when_unit_added_twice_then_logs_error);
    RUN_TEST(when_nonexistent_unit_removed_then_logs_error);
    // RUN_TEST(stress_test_many_units);
    UNITY_END();
    }
