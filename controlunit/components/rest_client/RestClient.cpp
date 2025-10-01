#include "RestClient.h"

RestClient::RestClient(const std::string& baseUrl, const std::string& jwtToken, int timeoutMs)
    : m_baseUrl(baseUrl), m_jwtToken { jwtToken }, m_timeout(timeoutMs) {}

esp_err_t RestClient::init() {
    esp_http_client_config_t config = {};
    config.url                      = m_baseUrl.c_str();
    config.timeout_ms               = m_timeout;
    config.keep_alive_enable        = true;
    // config.disable_auto_redirect    = false;
    // config.transport_type           = HTTP_TRANSPORT_OVER_SSL;

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

esp_err_t RestClient::postTo(const std::string& endpoint, const std::string& payload) {
        if (!m_client) return ESP_ERR_INVALID_STATE;

        std::string full_url = m_baseUrl + endpoint;
        esp_http_client_set_url(m_client, full_url.c_str());
        esp_http_client_set_method(m_client, HTTP_METHOD_POST);
        esp_http_client_set_post_field(m_client, payload.c_str(), payload.length());

        esp_err_t err = esp_http_client_perform(m_client);
        if (err == ESP_OK) {
            int status = esp_http_client_get_status_code(m_client);
            ESP_LOGI(TAG, "Svar %d från %s", status, endpoint.c_str());
        } else {
            ESP_LOGE(TAG, "Fel vid POST till %s: %s", endpoint.c_str(), esp_err_to_name(err));
        }

        return err;
    }




RestClient::~RestClient() {
    if (m_client) {
        esp_http_client_cleanup(m_client);
    }
}



// Gammalt bös troligen men vi kan se
esp_err_t RestClient::sendSensorData(float temp, int humidity) {
        if (!m_client) return ESP_ERR_INVALID_STATE;

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "{\"temperature\":%.2f,\"humidity\":%d}", temp, humidity);
        esp_http_client_set_post_field(m_client, buffer, strlen(buffer));

        esp_err_t err = esp_http_client_perform(m_client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Svar från server: %d", esp_http_client_get_status_code(m_client));
        } else {
            ESP_LOGE(TAG, "Fel vid skick: %s", esp_err_to_name(err));
        }

        return err;
    }

esp_err_t RestClient::sendJson(const std::string& endpoint,
                               const std::string& json_payload) {
    std::string full_url = m_baseUrl + endpoint;

    esp_http_client_config_t config = {
        .url        = full_url.c_str(),
        .method     = HTTP_METHOD_POST,
        .timeout_ms = m_timeout,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(
        client, json_payload.c_str(), json_payload.length());

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "Svar från %s: %d", full_url.c_str(), status);
    } else {
        ESP_LOGE(TAG,
                 "Fel vid skick till %s: %s",
                 full_url.c_str(),
                 esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return err;
}

// Exempelmetod: connect
esp_err_t RestClient::connectSensor(const std::string& sensor_id) {
    std::string payload =
        "{\"command\":\"connect\",\"sensor_id\":\"" + sensor_id + "\"}";
    return sendJson("/api/sensor/connect", payload);
}

// Exempelmetod: skicka sensordata
// esp_err_t RestClient::sendSensorData(float temp, int humidity) {
//     char buffer[128];
//     snprintf(buffer,
//              sizeof(buffer),
//              "{\"temperature\":%.2f,\"humidity\":%d}",
//              temp,
//              humidity);
//     return sendJson("/api/sensor/data", buffer);
// }