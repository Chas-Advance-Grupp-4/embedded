/**
 * @file postexample_handler.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Example of POST handler for http server
 *
 * Note that since the ESP-IDF use TLSF for heap allocations
 * it is safe to use std::string (as well as new/delete)
 *
 * @date 2025-09-12
 *
 */
#include "postexample_handler.h"
#include "esp_log.h"
#include <string>

static const char* TAG = "postexample_handler";

esp_err_t postexample_post_handler(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling /postexample request");

    int         total_len = req->content_len;
    std::string body;
    body.reserve(total_len);

    char buf[256];
    int  remaining = total_len;

    while (remaining > 0) {
        int to_read  = remaining > sizeof(buf) ? sizeof(buf) : remaining;
        int received = httpd_req_recv(req, buf, to_read);

        if (received <= 0) {
            ESP_LOGE(TAG, "Failed to receive POST data");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        body.append(buf, received);
        remaining -= received;
    }

    ESP_LOGI(TAG, "Received POST body: %s", body.c_str());

    httpd_resp_send(req, "POST received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}