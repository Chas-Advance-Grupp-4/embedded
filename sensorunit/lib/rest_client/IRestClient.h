/**
 * @file IRestClient.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Interface for Rest Client
 * @date 2025-10-15
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "constants.h"
#include <etl/string.h>

/**
 * @brief Broader than pure Rest Client http status
 * and includes internal errors (values < 0 ) not related
 * to server request
 *
 * * Currently unused, but to be implemented where applicable
 */
enum RestClientStatus {
    ClientSideError      = -7,
    BodyOverflow         = -6,
    InvalidResponse      = -5,
    HttpInitFailed       = -4,
    RequestFailed        = -3,
    Timeout              = -2,
    WifiNotConnected     = -1,
    Ok                   = 200,
    Created              = 201,
    Accepted             = 202,
    BadRequest           = 400,
    Unauthorized         = 401,
    Forbidden            = 403,
    NotFound             = 404,
    PayloadTooLarge      = 413,
    UnprocessableContent = 422,
    InternalServerError  = 500
};

/**
 * @brief A Rest Respons with http status and JSON payload response
 *
 */
struct RestResponse {
    int                                           status;
    etl::string<json_config::max_small_json_size> payload;
};

/**
 * @brief Interface for Rest Client
 *
 */
class IRestClient {
  public:
    virtual RestResponse getTo(const char* endpoint)                                   = 0;
    virtual RestResponse postTo(const char*                                   endpoint,
                                const etl::string<json_config::max_json_size> payload) = 0;
    virtual ~IRestClient()                                                             = default;
};