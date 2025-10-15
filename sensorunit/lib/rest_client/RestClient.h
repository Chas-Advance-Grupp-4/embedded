/**
 * @file RestClient.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Lightweight Rest Client for sending GET and POST requests
 * @date 2025-10-15
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "IRestClient.h"
#include <WiFiClient.h>
#include <WiFiS3.h>

class RestClient : public IRestClient {
  public:
    RestClient(const char*   baseUrl,
               uint16_t      port      = 8080,
               const char*   jwtToken  = "currently_not_used",
               unsigned long timeoutMs = 5000);

    /**
     * @brief Send GET request to the server
     *
     * @param endpoint should include slash, example: "/time"
     * @return RestResponse struct with status and payload
     */
    RestResponse getTo(const char* endpoint) override;

    /**
     * @brief Send POST request to the server
     * 
     * @param endpoint should include slash, example: "/connect"
     * @param payload a formated json string
     * @return RestResponse struct with status and payload
     */
    RestResponse postTo(const char*                                   endpoint,
                        const etl::string<json_config::max_json_size> payload) override;

  private:
    RestResponse                 parseResponse();
    int                          extractStatusCode(const etl::string<128>& statusLine);
    void                         sendGetHeader(const char* endpoint);
    void                         sendPostHeader(const char* endpoint, uint16_t contentLength);
    etl::string<32>              m_baseUrl;
    etl::string<64>              m_jwtToken;
    uint16_t                     m_port;
    WiFiClient                   m_client;
    unsigned long                m_timeout; /**< Timeout for HTTP requests in milliseconds. */
    static constexpr const char* TAG = "RestClient";
};