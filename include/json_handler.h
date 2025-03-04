#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_spiffs.h"
#include "cJSON.h"

#define TAG "JSON_HANDLER"
#define CONFIG_FILE "/spiffs/config.json"


// Estructura para almacenar la configuración
typedef struct {
    char device[32];
    struct {
        char ssid[32];
        char password[64];
        char ip[16];
        bool wifi_enable;
    } wifi;
    struct {
        bool mqtt_enable;
        char mqtt_server[64];
        int mqtt_port;
        int mqtt_qos;
        char mqtt_id[32];
        char mqtt_user[32];
        char mqtt_password[64];
    } mqtt;
} config_t;

// Variable global para almacenar la configuración
extern config_t global_config;


// Declaraciones de funciones
// Declaraciones de funciones
void init_spiffs(void);
void load_config(void);
void save_config(void);

#endif // JSON_HANDLER_H