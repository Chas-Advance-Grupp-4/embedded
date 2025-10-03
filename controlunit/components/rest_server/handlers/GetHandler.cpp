#include "GetHandler.h"
#include "esp_log.h"

static const char* TAG = "GetHandler";

GetHandler::GetHandler(const std::string& uri) 
    : m_uriString { uri } {
    m_uri = {
        .uri = m_uriString.c_str(),
        .method = HTTP_GET,
        .handler = GetHandler::staticHandler,
        .user_ctx = this
    };
}

httpd_uri_t* GetHandler::getUri() {
    return &m_uri;
}

esp_err_t GetHandler::staticHandler(httpd_req_t* req) {
    auto* self = static_cast<GetHandler*>(req->user_ctx);
    return self->handle(req);
}

esp_err_t GetHandler::handle(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling GET request for %s", m_uri.uri);
    return process(req);
}