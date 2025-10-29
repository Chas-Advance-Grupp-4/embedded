/**
 * @file RestClient.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Lightweight HTTPS client for posting JSON data to a remote endpoint.
 *
 * This class wraps the ESP-IDF HTTP client and provides simplified methods
 * for initialization and sending POST requests. It supports configurable
 * base URL, TLS, JWT token, and timeout settings
 * It uses the bundled TLS certificates that ESP-IDF provides
 *
 * @date 2025-10-07
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "RestClient.h"

RestClient::RestClient(const std::string& baseUrl,
                       const std::string& jwtToken,
                       int                timeoutMs)
    : m_baseUrl(baseUrl), m_jwtToken{jwtToken}, m_timeout(timeoutMs) {}

esp_err_t RestClient::init() {

    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == nullptr) {
        ESP_LOGE(TAG, "Failed to create mutex");
    }

    esp_http_client_config_t config = {};
    config.url                      = m_baseUrl.c_str();
    config.timeout_ms               = m_timeout;
    config.keep_alive_enable        = true;
    config.transport_type           = HTTP_TRANSPORT_OVER_SSL;
    config.crt_bundle_attach        = esp_crt_bundle_attach;
    config.event_handler            = &RestClient::httpEventHandler;
    config.user_data                = this;

    m_client = esp_http_client_init(&config);
    if (!m_client) {
        ESP_LOGE(TAG, "Could not initialize HTTP client");
        return ESP_FAIL;
    }

    esp_http_client_set_header(m_client, "Content-Type", "application/json");
    esp_err_t auth_err = esp_http_client_set_header(
        m_client, "Authorization", ("Bearer " + m_jwtToken).c_str());
    if (auth_err != ESP_OK) {
        ESP_LOGW(TAG,
                 "Set Authorization Header failed: %s",
                 esp_err_to_name(auth_err));
    }

    return ESP_OK;
}

esp_err_t RestClient::httpEventHandler(esp_http_client_event_t *evt) {
    auto* self = static_cast<RestClient*>(evt->user_data);

    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client)) {
                self->m_responseBody.append(static_cast<const char*>(evt->data), evt->data_len);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "Full response: %s", self->m_responseBody.c_str());
            break;
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP error occurred");
            break;
        default:
            break;
    }
    return ESP_OK;
}


RestClientResponse RestClient::postTo(const std::string& endpoint,
                                      const std::string& payload) {
    if (!m_mutex) {
        return {ESP_ERR_INVALID_STATE, ""};
    }
    m_responseBody.clear();

    ESP_LOGI(TAG, "Free heap: %u", esp_get_free_heap_size());

    // Mutex protected
    ESP_LOGI(TAG, "Taking mutex for POST to %s", endpoint.c_str());
    if (xSemaphoreTake(m_mutex, portMAX_DELAY) != pdTRUE) {
        // This will almost never happen since portMAX_DELAY waits forever
        return {ESP_ERR_TIMEOUT, ""};
    }
    std::string full_url = m_baseUrl + endpoint;
    esp_http_client_set_method(m_client, HTTP_METHOD_POST);
    esp_http_client_set_url(m_client, full_url.c_str());
    esp_http_client_set_post_field(m_client, payload.c_str(), payload.length());

    esp_err_t   err            = esp_http_client_perform(m_client);
    int         status         = esp_http_client_get_status_code(m_client);
        
    esp_http_client_close(
        m_client); // Close the socket but not the client itself
    xSemaphoreGive(m_mutex);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Response %d from %s", status, endpoint.c_str());
    } else {
        ESP_LOGE(TAG,
                 "Error at POST to %s: %s",
                 endpoint.c_str(),
                 esp_err_to_name(err));
    }    
    return {err, m_responseBody};
}

RestClient::~RestClient() {
    if (m_client) {
        esp_http_client_cleanup(m_client);
    }
}
