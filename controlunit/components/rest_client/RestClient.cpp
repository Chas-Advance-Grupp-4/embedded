/**
 * @file RestClient.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Lightweight HTTP client for posting JSON data to a remote endpoint.
 *
 * This class wraps the ESP-IDF HTTP client and provides simplified methods
 * for initialization and sending POST requests. It supports configurable
 * base URL, JWT token (optional), and timeout settings
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
    esp_http_client_config_t config = {};
    config.url                      = m_baseUrl.c_str();
    config.timeout_ms               = m_timeout;
    config.keep_alive_enable        = true;
    config.transport_type = HTTP_TRANSPORT_OVER_SSL;
    config.crt_bundle_attach = esp_crt_bundle_attach;

    m_client = esp_http_client_init(&config);
    if (!m_client) {
        ESP_LOGE(TAG, "Could not initialize HTTP client");
        return ESP_FAIL;
    }

    // esp_http_client_set_header(
    //     m_client, "Authorization", ("Bearer " + m_jwtToken).c_str());
    esp_http_client_set_header(m_client, "Content-Type", "application/json");

    return ESP_OK;
}

esp_err_t RestClient::postTo(const std::string& endpoint,
                             const std::string& payload) {
    if (!m_client)
        return ESP_ERR_INVALID_STATE;

    ESP_LOGI(TAG, "Free heap: %u", esp_get_free_heap_size());
    std::string full_url = m_baseUrl + endpoint;
    esp_http_client_set_url(m_client, full_url.c_str());
    esp_http_client_set_method(m_client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(m_client, payload.c_str(), payload.length());

    esp_err_t err = esp_http_client_perform(m_client);
    esp_http_client_close(m_client);    // Close the socket but not the client itself
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(m_client);
        ESP_LOGI(TAG, "Svar %d från %s", status, endpoint.c_str());
    } else {
        ESP_LOGE(TAG,
                 "Fel vid POST till %s: %s",
                 endpoint.c_str(),
                 esp_err_to_name(err));
    }

    return err;
}

RestClient::~RestClient() {
    if (m_client) {
        esp_http_client_cleanup(m_client);
    }
}
