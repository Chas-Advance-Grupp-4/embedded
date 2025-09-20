idf_component_register(
    SRCS "rest_server.cpp"
         "handlers/hello_handler.cpp"
         "handlers/status_handler.cpp"
         "handlers/postexample_handler.cpp"
    INCLUDE_DIRS "." "handlers"
    PRIV_REQUIRES esp_http_server esp_wifi log nvs_flash unity
)

if(CONFIG_UNIT_TEST_ENABLED)
    set(TEST_SRCS handlers/test/test_hello_handler.cpp)
    idf_component_add_unit_test(rest_server_tests "${TEST_SRCS}")
endif()
