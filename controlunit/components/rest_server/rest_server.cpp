#include "rest_server.h"
#include "hello_handler.h"
#include "status_handler.h"
#include "esp_http_server.h"
#include "esp_log.h"

static const char* TAG = "rest_server";

void start_rest_server() {
    ESP_LOGI(TAG, "Starting REST server");

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

        httpd_uri_t status_uri = {
            .uri      = "/status",
            .method   = HTTP_GET,
            .handler  = status_get_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &status_uri);

        ESP_LOGI(TAG, "Server started");
    } else {
        ESP_LOGE(TAG, "Failed to start server");
    }
}