extern "C" {
    #include "unity.h"
}
#include "hello_handler.h"

extern "C" void dummy_test_symbol(void) {}

extern "C" void test_hello_handler_returns_ok(void) {
    // You'd normally mock httpd_req_t and httpd_resp_send here
    esp_err_t result = hello_get_handler(nullptr); // Simplified
    TEST_ASSERT_EQUAL(ESP_OK, result);
}
