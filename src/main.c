// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "smartconfig_ack.h"

// #include <string.h>

// #define TAG "SmartConfig"

// static EventGroupHandle_t wifi_event_group;
// const int CONNECTED_BIT = BIT0;
// const int ESPTOUCH_DONE_BIT = BIT1;

// static void smartconfig_task(void * param);
// static void wifi_init_task(void *param);

// static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         xTaskCreate(smartconfig_task, "smartconfig_task", 4096, NULL, 5, NULL);
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         esp_wifi_connect();
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
//         ESP_LOGI(TAG, "Scan done");
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
//         ESP_LOGI(TAG, "Channel found");
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
//         smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
//         wifi_config_t wifi_config;
//         memset(&wifi_config, 0, sizeof(wifi_config_t));
//         memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
//         memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));

//         ESP_LOGI(TAG, "SSID: %s", wifi_config.sta.ssid);
//         ESP_LOGI(TAG, "Password: %s", wifi_config.sta.password);

//         esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
//         esp_wifi_connect();
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
//         xEventGroupSetBits(wifi_event_group, ESPTOUCH_DONE_BIT);
//     }
// }

// static void smartconfig_task(void * param) {
//     ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
//     smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
//     while (1) {
//         EventBits_t bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
//         if (bits & CONNECTED_BIT) {
//             ESP_LOGI(TAG, "Connected to AP");
//         }
//         if (bits & ESPTOUCH_DONE_BIT) {
//             ESP_LOGI(TAG, "SmartConfig done");
//             esp_smartconfig_stop();
//             vTaskDelete(NULL);
//         }
//     }
// }

// static void wifi_init_task(void *param) {
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);

//     wifi_event_group = xEventGroupCreate();
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = "",
//             .password = ""
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
//     ESP_ERROR_CHECK(esp_wifi_start());
//     vTaskDelete(NULL);
// }

// void app_main(void) {
//     xTaskCreate(wifi_init_task, "wifi_init_task", 4096, NULL, 5, NULL);
// }



#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wifi_smartconfig.h"

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
    ESP_LOGI(TAG, "Iniciando...");
    wifi_init(wifi_event_handler);
}

