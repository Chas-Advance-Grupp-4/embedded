#pragma once
#include <ctime>

namespace json_config{
    constexpr size_t max_batch_size = 10;
    constexpr size_t max_json_size = 1024;
    constexpr size_t max_json_doc_size = max_json_size * 2;
}