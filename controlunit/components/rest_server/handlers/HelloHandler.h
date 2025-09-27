#pragma once
#include "BaseHandler.h"
#include "esp_log.h"
#include <string>

class HelloHandler : public BaseHandler {
  public:
    HelloHandler(const char* uri);
    httpd_uri_t* getUri() override;
    esp_err_t    handle(httpd_req_t* req);

  private:
    std::string      m_greeting;
    httpd_uri_t      m_uri;
    static esp_err_t staticHandler(httpd_req_t* req);
};