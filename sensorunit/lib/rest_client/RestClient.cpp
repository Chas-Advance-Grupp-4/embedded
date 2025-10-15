/**
 * @file RestClient.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of RestClient class
 * @date 2025-10-15
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "RestClient.h"
#include "logging.h"

RestClient::RestClient(const char*   baseUrl,
                       uint16_t      port,
                       const char*   jwtToken,
                       unsigned long timeoutMs)
    : m_baseUrl{baseUrl}, m_port{port}, m_jwtToken{jwtToken}, m_timeout{timeoutMs} {}

RestResponse RestClient::getTo(const char* endpoint) {

    LOG_INFO(TAG, "Sending GET request to %s", endpoint);
    // Open connection
    if (!m_client.connect(m_baseUrl.c_str(), m_port)) {
        LOG_ERROR(TAG, "Failed to connect to %s", m_baseUrl.c_str());
        return {-1, ""};
    }

    sendGetHeader(endpoint);

    RestResponse response = parseResponse();

    // Close connection
    m_client.stop();

    return response;
}


RestResponse RestClient::postTo(const char*                                   endpoint,
                                const etl::string<json_config::max_json_size> payload) {
    
    LOG_INFO(TAG, "Sending POST request to %s", endpoint);
    // Open connection
    if (!m_client.connect(m_baseUrl.c_str(), m_port)) {
        LOG_ERROR(TAG, "Failed to connect to %s", m_baseUrl.c_str());
        return {-1, ""};
    }

    // Send
    uint16_t contentLength = payload.size();
    sendPostHeader(endpoint, contentLength);
    m_client.print(payload.c_str());

    // Get response
    RestResponse response = parseResponse();

    // Close connection
    m_client.stop();

    return response;
}

int RestClient::extractStatusCode(const etl::string<128>& statusLine) {
    int protocolEnd   = statusLine.find(' ');
    int statusCodeEnd = statusLine.find(' ', protocolEnd + 1);
    if (protocolEnd != etl::string<128>::npos && statusCodeEnd != etl::string<128>::npos) {
        auto codeStr = statusLine.substr(protocolEnd + 1, statusCodeEnd - protocolEnd - 1);
        return atoi(codeStr.c_str());
    }
    return -1;
}

void RestClient::sendGetHeader(const char* endpoint) {
    m_client.print("GET ");
    m_client.print(endpoint);
    m_client.println(" HTTP/1.1");

    m_client.print("Host: ");
    m_client.println(m_baseUrl.c_str());

    m_client.println("Connection: close");
    m_client.println(); /// Empty line ends header
}

void RestClient::sendPostHeader(const char* endpoint, uint16_t contentLength) {
    m_client.print("POST ");
    m_client.print(endpoint);
    m_client.println(" HTTP/1.1");

    m_client.print("Host: ");
    m_client.println(m_baseUrl.c_str());

    m_client.println("Content-Type: application/json");

    m_client.print("Content-Length: ");
    m_client.println(contentLength);

    m_client.println("Connection: close");
    m_client.println(); /// Empty line ends header
}

RestResponse RestClient::parseResponse() {
    RestResponse                                   response;
    int                                            statusCode{};
    etl::string<json_config::max_header_line_size> headerLine;
    etl::string<json_config::max_small_json_size>  body;
    bool                                           headersEnded = false;
    unsigned long                                  start        = millis();

    while (m_client.connected() && (millis() - start < m_timeout)) {
        while (m_client.available()) {
            char c = m_client.read();

            if (!headersEnded) {
                if (c == '\n') {
                    if (headerLine.ends_with("\r"))
                        headerLine.pop_back();
                    if (headerLine.empty()) {
                        headersEnded = true;
                    } else if (headerLine.starts_with("HTTP/1.")) {
                        statusCode = extractStatusCode(headerLine);
                    }
                    headerLine.clear();
                } else {
                    if (!headerLine.full())
                        headerLine.push_back(c);
                }
            } else {
                if (!body.full()) {
                    body.push_back(c);                    
                } else {
                    LOG_WARN(TAG, "Response body exceeding max size: %d", json_config::max_small_json_size);
                    break;
                }
            }
        }
    }

    response.payload = body;
    if (body.full()) {
        response.status = -6;        
    } else {
        response.status  = statusCode;
    }
    return response;
}

