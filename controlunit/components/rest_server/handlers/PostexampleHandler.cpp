#include "PostexampleHandler.h"
#include "esp_log.h"

static const char* TAG = "PostexampleHandler";

PostexampleHandler::PostexampleHandler(const char* uri)
    : PostHandler(uri) {}

esp_err_t PostexampleHandler::processBody(httpd_req_t* req, const std::string& body) {
    ESP_LOGI(TAG, "Processing POST body in PostexampleHandler");

    // TODO: Parse JSON and delegate to appropriate logic
    // For now, just echo back
    httpd_resp_set_type(req, "text/plain; charset=utf-8");
    httpd_resp_send(req, "POST was received", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}