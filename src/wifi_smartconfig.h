#ifndef WIFI_SMARTCONFIG_H
#define WIFI_SMARTCONFIG_H

#include "esp_wifi_types.h"  // Incluir la definición de wifi_event_t

typedef void (*wifi_callback_t)(wifi_event_t event, void *arg);

void wifi_init(wifi_callback_t cb);

#endif