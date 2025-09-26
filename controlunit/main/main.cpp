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
    nvs_flash_init();
    init_wifi();
    RestServer server;
    if (server.start()) {
        // Possible additional LOG message here
    }
}