#pragma once
#include "IRestClient.h"
#include <WiFiS3.h>
#include <WiFiClient.h>

class RestClient : public IRestClient {
  public:
    RestClient(const char* baseUrl, uint16_t port = 8080,
               const char* jwtToken = "currently_not_used",
               unsigned long timeoutMs = 5000);

    RestResponse getTo(const char* endpoint) override;
    RestResponse postTo(const char* endpoint, const etl::string<json_config::max_json_size> payload) override;

    private:
    RestResponse parseResponse();
    int extractStatusCode(const etl::string<128>& statusLine);
    etl::string<32> m_baseUrl; 
    etl::string<64> m_jwtToken; 
    uint16_t m_port;
    WiFiClient m_client;
    unsigned long m_timeout; /**< Timeout for HTTP requests in milliseconds. */
    static constexpr const char* TAG = "RestClient";
};