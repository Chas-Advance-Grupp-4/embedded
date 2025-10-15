#include "RestClient.h"
#include "logging.h"

RestClient::RestClient(const char*   baseUrl,
                       uint16_t      port,
                       const char*   jwtToken,
                       unsigned long timeoutMs)
    : m_baseUrl{baseUrl}, m_port{port}, m_jwtToken{jwtToken}, m_timeout{timeoutMs} {}

RestResponse RestClient::getTo(const char* endpoint) {
    etl::string<64> fullUrl = m_baseUrl;
    fullUrl.append(endpoint);

    if (!m_client.connect(m_baseUrl.c_str(), 8080)) {
        LOG_ERROR(TAG, "Failed to connect to %s", m_baseUrl.c_str());
        return {-1, ""}; // fixa enumen!!!!
    }

    m_client.print("GET ");
    m_client.print(endpoint);
    m_client.println(" HTTP/1.1");

    m_client.print("Host: ");
    m_client.println(m_baseUrl.c_str());

    // if (!m_jwtToken.empty()) {
    //     m_client.print("Authorization: Bearer ");
    //     m_client.println(m_jwtToken.c_str());
    // }

    m_client.println("Connection: close");
    m_client.println(); // tom rad avslutar headers

    RestResponse response = parseResponse();

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

RestResponse RestClient::parseResponse() {
    RestResponse                                  response;
    int                                           statusCode{};
    etl::string<128>                              headerLine;
    etl::string<json_config::max_small_json_size> body;
    bool                                          headersEnded = false;
    unsigned long                                 start        = millis();

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
                if (!body.full())
                    body.push_back(c);
            }
        }
    }

    response.payload = body;
    response.status  = statusCode; // Fixa detta sedan
    // response.status = (response.statusCode == 200) ? RestClientStatus::Ok :
    // RestClientStatus::ServerError;
    return response;
}

RestResponse RestClient::postTo(const char*                                   endpoint,
                                const etl::string<json_config::max_json_size> payload) {
    return RestResponse{-1, ""};
}