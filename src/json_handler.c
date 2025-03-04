#include "json_handler.h"

// Variable global para almacenar la configuración
config_t global_config;

// Función para inicializar SPIFFS
void init_spiffs() {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "SPIFFS mounted successfully");
}

// Función para cargar la configuración desde el archivo JSON
void load_config() {
    FILE* f = fopen(CONFIG_FILE, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *json_string = malloc(fsize + 1);
    fread(json_string, 1, fsize, f);
    fclose(f);
    json_string[fsize] = 0;

    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        ESP_LOGE(TAG, "Failed to parse JSON");
        free(json_string);
        return;
    }

    // Cargar datos del JSON a la estructura global
    cJSON *device = cJSON_GetObjectItem(root, "device");
    if (device) strncpy(global_config.device, device->valuestring, sizeof(global_config.device));

    cJSON *wifi = cJSON_GetObjectItem(root, "wifi");
    if (wifi) {
        cJSON *ssid = cJSON_GetObjectItem(wifi, "ssid");
        if (ssid) strncpy(global_config.wifi.ssid, ssid->valuestring, sizeof(global_config.wifi.ssid));

        cJSON *password = cJSON_GetObjectItem(wifi, "password");
        if (password) strncpy(global_config.wifi.password, password->valuestring, sizeof(global_config.wifi.password));

        cJSON *ip = cJSON_GetObjectItem(wifi, "ip");
        if (ip) strncpy(global_config.wifi.ip, ip->valuestring, sizeof(global_config.wifi.ip));

        cJSON *wifi_enable = cJSON_GetObjectItem(wifi, "wifi_enable");
        if (wifi_enable) global_config.wifi.wifi_enable = cJSON_IsTrue(wifi_enable);
    }

    cJSON *mqtt = cJSON_GetObjectItem(root, "mqtt");
    if (mqtt) {
        cJSON *mqtt_enable = cJSON_GetObjectItem(mqtt, "mqtt_enable");
        if (mqtt_enable) global_config.mqtt.mqtt_enable = cJSON_IsTrue(mqtt_enable);

        cJSON *mqtt_server = cJSON_GetObjectItem(mqtt, "mqtt_server");
        if (mqtt_server) strncpy(global_config.mqtt.mqtt_server, mqtt_server->valuestring, sizeof(global_config.mqtt.mqtt_server));

        cJSON *mqtt_port = cJSON_GetObjectItem(mqtt, "mqtt_port");
        if (mqtt_port) global_config.mqtt.mqtt_port = mqtt_port->valueint;

        cJSON *mqtt_qos = cJSON_GetObjectItem(mqtt, "mqtt_qos");
        if (mqtt_qos) global_config.mqtt.mqtt_qos = mqtt_qos->valueint;

        cJSON *mqtt_id = cJSON_GetObjectItem(mqtt, "mqtt_id");
        if (mqtt_id) strncpy(global_config.mqtt.mqtt_id, mqtt_id->valuestring, sizeof(global_config.mqtt.mqtt_id));

        cJSON *mqtt_user = cJSON_GetObjectItem(mqtt, "mqtt_user");
        if (mqtt_user) strncpy(global_config.mqtt.mqtt_user, mqtt_user->valuestring, sizeof(global_config.mqtt.mqtt_user));

        cJSON *mqtt_password = cJSON_GetObjectItem(mqtt, "mqtt_password");
        if (mqtt_password) strncpy(global_config.mqtt.mqtt_password, mqtt_password->valuestring, sizeof(global_config.mqtt.mqtt_password));
    }

    cJSON_Delete(root);
    free(json_string);
    ESP_LOGI(TAG, "Config loaded successfully");
}

// Función para guardar la configuración en el archivo JSON
void save_config() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "device", global_config.device);

    cJSON *wifi = cJSON_CreateObject();
    cJSON_AddStringToObject(wifi, "ssid", global_config.wifi.ssid);
    cJSON_AddStringToObject(wifi, "password", global_config.wifi.password);
    cJSON_AddStringToObject(wifi, "ip", global_config.wifi.ip);
    cJSON_AddBoolToObject(wifi, "wifi_enable", global_config.wifi.wifi_enable);
    cJSON_AddItemToObject(root, "wifi", wifi);

    cJSON *mqtt = cJSON_CreateObject();
    cJSON_AddBoolToObject(mqtt, "mqtt_enable", global_config.mqtt.mqtt_enable);
    cJSON_AddStringToObject(mqtt, "mqtt_server", global_config.mqtt.mqtt_server);
    cJSON_AddNumberToObject(mqtt, "mqtt_port", global_config.mqtt.mqtt_port);
    cJSON_AddNumberToObject(mqtt, "mqtt_qos", global_config.mqtt.mqtt_qos);
    cJSON_AddStringToObject(mqtt, "mqtt_id", global_config.mqtt.mqtt_id);
    cJSON_AddStringToObject(mqtt, "mqtt_user", global_config.mqtt.mqtt_user);
    cJSON_AddStringToObject(mqtt, "mqtt_password", global_config.mqtt.mqtt_password);
    cJSON_AddItemToObject(root, "mqtt", mqtt);

    char *json_string = cJSON_Print(root);
    FILE* f = fopen(CONFIG_FILE, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        cJSON_Delete(root);
        free(json_string);
        return;
    }
    fprintf(f, "%s", json_string);
    fclose(f);
    cJSON_Delete(root);
    free(json_string);
    ESP_LOGI(TAG, "Config saved successfully");
}


