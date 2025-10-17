idf_component_register(
    SRCS "RestServer.cpp"
         "handlers/BaseHandler.cpp"
         "handlers/PostHandler.cpp"
         "handlers/GetHandler.cpp"
         "handlers/PostexampleHandler.cpp"
         "handlers/HelloHandler.cpp"
    INCLUDE_DIRS "." "handlers"
    REQUIRES esp_http_server esp_wifi log nvs_flash unity
)

if(CONFIG_UNIT_TEST_ENABLED)
    set(TEST_SRCS handlers/test/test_hello_handler.cpp)
    idf_component_add_unit_test(rest_server_tests "${TEST_SRCS}")
endif()
