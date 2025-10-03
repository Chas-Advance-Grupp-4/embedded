#include "MockDataGenerator.h"
#include "ReadingsDispatcher.h"
#include "RestClient.h"
#include "RestServer.h"
#include "wifi_config.h"
#include "wifi_manager.h"
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

extern "C" void app_main(void) {
    // Wait for monitor so we don't miss first part of the log
    vTaskDelay(pdMS_TO_TICKS(500));

    nvs_flash_init();
    init_wifi();
    static RestServer server;
    if (server.start()) {
        // Possible additional LOG message here
    }

    static RestClient client("http://192.168.1.129:8080",
                             "eyJhbGciOiJIUzI1NiIs...");
    client.init();
    client.postTo("/post", "{\"content\":\"Hello from ESP32\"}");
    static ControlUnitManager manager;
    vTaskDelay(pdMS_TO_TICKS(500));

    static MockDataGenerator mockdataGenerator(manager, 5'000'000);
    mockdataGenerator.start();
    // Wait so we have time to see the first post on the server
    vTaskDelay(pdMS_TO_TICKS(8000));

    static ReadingsDispatcher dispatcher(client, manager, 30'000'000);
    dispatcher.start();
}