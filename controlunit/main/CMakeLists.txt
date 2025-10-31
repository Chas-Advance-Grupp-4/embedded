idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."   
    PRIV_REQUIRES 
        net_utils 
        rest_server 
        rest_client 
        readings_dispatcher 
        mock_data 
        esp_http_server 
        esp_wifi 
        log 
        nvs_flash
        time_sync_manager
        sensor_unit_link_syncer
)