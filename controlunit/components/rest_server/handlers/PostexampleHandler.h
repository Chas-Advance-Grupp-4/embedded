#pragma once
#include "PostHandler.h"

class PostexampleHandler : public PostHandler {
public:
    PostexampleHandler(const std::string& uri);

protected:
    esp_err_t processBody(httpd_req_t* req, const std::string& body) override;
};