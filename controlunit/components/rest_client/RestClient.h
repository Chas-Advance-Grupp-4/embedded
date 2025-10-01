#pragma once

#include "esp_http_client.h"
#include "esp_log.h"
#include <string>

// RestClient
class RestClient {
public:
    RestClient(const std::string& base_url, const std::string& jwt_token, int timeout_ms = 5000);
    ~RestClient();
    
    esp_err_t init();
    esp_err_t postTo(const std::string& endpoint, const std::string& payload);

    // Andra utgåvan
    esp_err_t sendSensorData(float temp, int humidity);

    // Första utgåvan
    esp_err_t sendJson(const std::string& endpoint, const std::string& json_payload);        
    // Exempelmetod: connect
    esp_err_t connectSensor(const std::string& sensor_id); 
    // Exempelmetod: skicka sensordata
    // esp_err_t sendSensorData(float temp, int humidity); 

private:
    std::string m_baseUrl;
    std::string m_jwtToken;    // unused for now
    esp_http_client_handle_t m_client;
    int m_timeout;
    static constexpr const char* TAG = "RestClient";
};