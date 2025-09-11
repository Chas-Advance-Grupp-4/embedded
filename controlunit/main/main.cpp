#include "wifi_config.h"
#include "wifi_manager.h"
#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <nvs_flash.h>




static const char *TAG = "web_server";

esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = "Hello from separate files ESP32!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "Starting Wi-Fi");
    nvs_flash_init();  // Required for Wi-Fi and NVS
    init_wifi();

    ESP_LOGI(TAG, "Starting HTTP server");

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t hello_uri = {
            .uri      = "/hello",
            .method   = HTTP_GET,
            .handler  = hello_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &hello_uri);
        ESP_LOGI(TAG, "Server started");
    } else {
        ESP_LOGE(TAG, "Failed to start server");
    }
}