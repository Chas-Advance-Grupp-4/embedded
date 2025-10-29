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
 * @brief Initializes the ESP32 Wi-Fi stack in dual-mode (Access Point + Station).
 *
 * This function sets up the ESP32 Wi-Fi stack in both Access Point (AP) and Station (STA) modes.
 * It restores any saved Wi-Fi configuration from NVS using `esp_wifi_restore()` and initializes
 * the network interfaces and event loop.
 *
 * For the AP interface, it configures a static IP address, gateway, and netmask, and sets up WPA2
 * authentication using credentials defined in `wifi_config.h`. The STA interface is configured with
 * SSID and password, and attempts to connect to the external network.
 *
 * Event handlers are registered for IP acquisition, connection, and disconnection events.
 * FreeRTOS event groups are used to wait for successful STA connection, and IP information is logged
 * upon acquisition.
 *
 * This function is intended to be called only once during system startup.
 * It blocks until the STA interface has successfully acquired an IP address.
 */
void init_wifi();
