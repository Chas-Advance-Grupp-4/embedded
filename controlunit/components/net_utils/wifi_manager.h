/**
 * @file wifi_manager.h
 * @brief Initializes and manages dual-mode Wi-Fi (AP + STA) for ESP32.
 *
 * Provides a single entry point to configure and start the ESP32 Wi-Fi stack
 * in both Access Point (AP) and Station (STA) modes. Uses credentials and IP
 * settings defined in `wifi_config.h`.
 *
 * Registers event handlers for IP acquisition and disconnection, and uses
 * FreeRTOS event groups to synchronize connection status.
 *
 * The AP is configured with a static IP and WPA2 security, while the STA
 * connects to an external Wi-Fi network.
 *
 * @author Erik Dahl (erik@iunderlandet.se)
 * @date 2025-10-07
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 */
#pragma once

/**
 * @brief Initializes Wi-Fi in dual-mode (AP + STA) and connects to the configured networks.
 *
 * Sets up the ESP32 Wi-Fi stack in both Access Point (AP) and Station (STA) modes.
 * Configures static IP settings and WPA2 authentication for the AP interface using
 * credentials from `wifi_config.h`. Initializes the STA interface with SSID and password,
 * and attempts to connect to the external network.
 *
 * Registers event handlers for IP acquisition and disconnection, and uses FreeRTOS
 * event groups to wait for successful STA connection.
 *
 * This function blocks until the STA interface has acquired an IP address.
 */
void init_wifi();
