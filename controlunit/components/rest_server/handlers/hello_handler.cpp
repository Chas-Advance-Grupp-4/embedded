#include "hello_handler.h"
#include "esp_log.h"

static const char* TAG = "hello_handler";

esp_err_t hello_get_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling /hello request");
    const char* resp_str = "Hello from ESP32 rest server!";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}