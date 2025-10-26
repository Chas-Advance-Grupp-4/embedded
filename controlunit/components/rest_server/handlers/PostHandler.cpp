/**
 * @file PostHandler.cpp
 * @brief Implementation of the PostHandler base class for HTTPS POST request handling.
 *
 * Defines the logic for routing POST requests to a subclass-defined `processBody()` method.
 * Includes static and instance-level handlers for reading the request body and invoking
 * custom processing logic.
 *
 * Used by RestServer to register POST endpoints with dynamic payload handling.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#include "PostHandler.h"
#include "esp_log.h"

static const char* TAG = "PostHandler";

PostHandler::PostHandler(const std::string& uri) 
    : m_uriString { uri } {
    m_uri = {
        .uri = m_uriString.c_str(),
        .method = HTTP_POST,
        .handler = PostHandler::staticHandler,
        .user_ctx = this
    };
}

httpd_uri_t* PostHandler::getUri() {
    return &m_uri;
}

esp_err_t PostHandler::staticHandler(httpd_req_t* req) {
    auto* self = static_cast<PostHandler*>(req->user_ctx);
    return self->handle(req);
}

esp_err_t PostHandler::handle(httpd_req_t* req) {
    ESP_LOGI(TAG, "Handling POST request for %s", m_uri.uri);

    int total_len = req->content_len;
    std::string body;
    body.reserve(total_len);

    char buf[256];
    int remaining = total_len;

    while (remaining > 0) {
        int to_read = remaining > sizeof(buf) ? sizeof(buf) : remaining;
        int received = httpd_req_recv(req, buf, to_read);

        if (received <= 0) {
            ESP_LOGE(TAG, "Failed to receive POST data");
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }

        body.append(buf, received);
        remaining -= received;
    }

    ESP_LOGI(TAG, "Received body: %s", body.c_str());
    return processBody(req, body);
}