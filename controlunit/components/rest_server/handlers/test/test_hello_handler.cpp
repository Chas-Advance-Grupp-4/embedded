/**
 * @brief Test file for hello_handler.cpp
 * 
 * This is just an example test
 * 
 */
extern "C" {
    #include "unity.h"
}
#include "hello_handler.h"

extern "C" void test_hello_handler_returns_ok(void) {
    esp_err_t result = hello_get_handler(nullptr); 
    TEST_ASSERT_EQUAL(ESP_OK, result);
}
