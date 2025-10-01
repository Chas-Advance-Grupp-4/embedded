#pragma once
#include "BaseHandler.h"
#include "esp_http_server.h"
#include <string>

class GetHandler : public BaseHandler {
public:
    httpd_uri_t* getUri() override;

protected:
    GetHandler(const std::string& uri);
    virtual esp_err_t process(httpd_req_t* req) = 0;

private:
    std::string m_uriString;
    httpd_uri_t m_uri;
    static esp_err_t staticHandler(httpd_req_t* req);
    esp_err_t handle(httpd_req_t* req);
};