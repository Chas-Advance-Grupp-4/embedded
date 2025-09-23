idf_component_register(
    SRCS 
        "main.cpp"
        "../../components/rest_server/handlers/test/test_hello_handler.cpp"
        "../../components/sensor_unit_manager/test/test_SensorUnitManager.cpp"
    INCLUDE_DIRS "."   
    PRIV_REQUIRES sensor_unit_manager rest_server log esp_http_server unity
)