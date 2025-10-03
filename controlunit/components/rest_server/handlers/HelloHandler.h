#pragma once
#include "GetHandler.h"
#include "esp_log.h"
#include <string>

class HelloHandler : public GetHandler {
  public:
    HelloHandler(const std::string& uri);

  protected:
    esp_err_t process(httpd_req_t* req) override;

  private:
  std::string m_greeting;
};