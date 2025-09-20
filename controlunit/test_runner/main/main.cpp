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


extern "C" void app_main() {
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI("TEST", "Test-runner startar...");
     UNITY_BEGIN();
     RUN_TEST(test_hello_handler_returns_ok);
     UNITY_END();
}
