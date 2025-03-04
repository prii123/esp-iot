#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_smartconfig.h"

#include "json_handler.h"
#include "esp_wifi_types.h"


#define TAG "MAIN"

void wifi_event_handler(wifi_event_t event, void *arg) {
    switch (event) {
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Conectado al WiFi. IP: %s", (char *)arg);
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "WiFi desconectado. Intentando reconectar...");
            break;
        default:
            break;
    }
}

void app_main(void) {
    // Inicializar SPIFFS
    init_spiffs();

    ESP_LOGI(TAG, "Iniciando...");

    // Cargar configuración al inicio
    load_config();

    // Ejemplo: Cambiar el SSID y guardar
    strncpy(global_config.wifi.ssid, "NuevoSSID", sizeof(global_config.wifi.ssid));
    save_config();

    // Ejemplo: Leer el SSID actual
    ESP_LOGI(TAG, "SSID actual: %s", global_config.wifi.ssid);

    wifi_init(wifi_event_handler);


}

