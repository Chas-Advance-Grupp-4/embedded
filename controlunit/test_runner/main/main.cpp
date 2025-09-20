extern "C" {
    #include <unity.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <esp_log.h>
}
// #include "../../components/rest_server/handlers/hello_handler.h"

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

// extern "C" void app_main() {
//     printf("Test-runner startar...\n");
//     UNITY_BEGIN();
//     RUN_TEST(test_hello_handler_returns_ok);
//     UNITY_END();
// }