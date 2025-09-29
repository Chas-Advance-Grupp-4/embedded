#pragma once

#include "esp_http_server.h"

class BaseHandler {
public:
    virtual ~BaseHandler() = default;
    virtual httpd_uri_t* getUri() = 0;
};