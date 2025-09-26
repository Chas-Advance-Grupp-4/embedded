#include "status_handler.h"
#include "esp_log.h"

static const char* TAG = "status_handler";

esp_err_t status_get_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling /status request");
    const char* resp_str = "Status: OK";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}