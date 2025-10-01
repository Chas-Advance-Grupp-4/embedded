#pragma once
#include "BaseHandler.h"
#include "esp_http_server.h"
#include <string>

class PostHandler : public BaseHandler {
public:
    httpd_uri_t* getUri() override;

protected:
    PostHandler(const std::string& uri);
    virtual esp_err_t processBody(httpd_req_t* req, const std::string& body) = 0;

private:
    std::string m_uriString;    // for ownership of the uri string
    httpd_uri_t m_uri;
    static esp_err_t staticHandler(httpd_req_t* req);
    esp_err_t handle(httpd_req_t* req);
};