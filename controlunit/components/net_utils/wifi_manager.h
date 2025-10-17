/**
 * @file wifi_manager.h
 * @brief Initializes and manages Wi-Fi connection for ESP32 in station mode.
 *
 * Provides a single entry point to configure and connect to a Wi-Fi network
 * using credentials defined in `wifi_config.h`. Registers event handlers for
 * IP acquisition and disconnection.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once

/**
 * @brief Initializes Wi-Fi and connects to the configured SSID.
 *
 * Sets up the network interface, registers event handlers, and starts
 * the Wi-Fi driver in station mode. Blocks until a connection is established.
 */
void init_wifi();
