idf_component_register(
    SRCS 
        "main.cpp"
        "../../components/sensor_unit_manager/test/test_SensorUnitManager.cpp"
        "../../components/json_parser/test/test_JsonParser.cpp"
        "../../components/connection_data/test/test_connection_data_types.cpp"
    INCLUDE_DIRS "."   
    PRIV_REQUIRES sensor_unit_manager rest_server log esp_http_server json_parser connection_data unity
)