#include "hello_handler.h"

static const char* TAG = "hello_handler";

esp_err_t hello_get_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling /hello request");
    etl::string<64> resp_str = "Hello from ETL powered ESP32 rest server!";
    httpd_resp_send(req, resp_str.c_str(), resp_str.size());
    return ESP_OK;
}