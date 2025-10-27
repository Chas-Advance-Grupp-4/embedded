/**
 * @file main.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Main entry point for the Control Unit app
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "MockDataGenerator.h"
#include "ReadingsDispatcher.h"
#include "RestClient.h"
#include "RestServer.h"
#include "TimeSyncManager.h"
#include "wifi_config.h"
#include "wifi_manager.h"
#include <esp_event.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <nvs_flash.h>

extern "C" void app_main(void) {
    /// Wait for monitor so we don't miss first part of the log
    vTaskDelay(pdMS_TO_TICKS(500));
    nvs_flash_init();
    esp_wifi_restore();
    init_wifi();

    static TimeSyncManager timeSyncManager;
    timeSyncManager.start();

    static SensorUnitManager sensorUnitManager;
    sensorUnitManager.init();
    // Connect Sensor Unit manually
    sensorUnitManager.addUnit(Uuid(TEST_SENSOR_UNIT_ID));

    static RestServer server(
        CONTROL_UNIT_PORT, timeSyncManager, sensorUnitManager);
    if (server.start()) {
    }

    static RestClient client(CLIENT_URL, "eyJhbGciOiJIUzI1NiIs...");
    client.init();
    client.postTo("/post", "{\"content\":\"Hello from ESP32\"}");
    static ControlUnitManager manager(sensorUnitManager);
    vTaskDelay(pdMS_TO_TICKS(500));

    // static MockDataGenerator mockdataGenerator(manager, 5'000'000);
    // mockdataGenerator.start();

    vTaskDelay(pdMS_TO_TICKS(200));

    static ReadingsDispatcher dispatcher(client, manager, 30'000'000);
    dispatcher.start();
}