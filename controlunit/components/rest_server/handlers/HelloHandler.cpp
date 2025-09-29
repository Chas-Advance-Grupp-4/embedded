#include "HelloHandler.h"

static const char* TAG = "HelloHandler";

HelloHandler::HelloHandler(const char* uri)
    : m_greeting("Hello from ESP32 Still in progress") {
    m_uri = {.uri      = uri,
             .method   = HTTP_GET,
             .handler  = HelloHandler::staticHandler,
             .user_ctx = this};
}

httpd_uri_t* HelloHandler::getUri() {
    return &m_uri;
}

esp_err_t HelloHandler::handle(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling /hello request");
    httpd_resp_set_type(req, "text/plain; charset=utf-8");
    httpd_resp_send(req, m_greeting.c_str(), m_greeting.length());
    return ESP_OK;
}

esp_err_t HelloHandler::staticHandler(httpd_req_t* req) {
    auto* self = static_cast<HelloHandler*>(req->user_ctx);
    return self->handle(req);
}
