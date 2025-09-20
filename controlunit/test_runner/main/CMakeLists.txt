idf_component_register(
    SRCS 
        "main.cpp"
        "../../components/rest_server/handlers/test/test_hello_handler.cpp"
    INCLUDE_DIRS "."   
    PRIV_REQUIRES rest_server log esp_http_server unity
)