#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_netif_sntp.h"
#include "esp_wifi.h"
#include "protocol_examples_common.h"

static const char *TAG = "IOT_NTP_IFPB";

static void print_ap_info(void)
{
    wifi_ap_record_t ap_info;
    ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&ap_info));

    ESP_LOGI(TAG, "--- Informacoes do AP ---");
    ESP_LOGI(TAG, "SSID: %s", (char *)ap_info.ssid);
    ESP_LOGI(TAG, "BSSID: %02x:%02x:%02x:%02x:%02x:%02x",
             ap_info.bssid[0], ap_info.bssid[1], ap_info.bssid[2],
             ap_info.bssid[3], ap_info.bssid[4], ap_info.bssid[5]);
    ESP_LOGI(TAG, "Canal primario: %d", ap_info.primary);
    ESP_LOGI(TAG, "RSSI: %d dBm", ap_info.rssi);
}

static void init_sntp(void)
{
    ESP_LOGI(TAG, "Inicializando SNTP...");

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.br");
    ESP_ERROR_CHECK(esp_netif_sntp_init(&config));

    ESP_LOGI(TAG, "Aguardando sincronizacao com o pool.ntp.br...");

    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(15000)) != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao sincronizar o horario via SNTP");
        return;
    }

    ESP_LOGI(TAG, "Horario sincronizado com sucesso");

    // UTC-3 (horario de Brasilia / Paraiba)
    setenv("TZ", "<-03>3", 1);
    tzset();
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI(TAG, "Conectando ao Wi-Fi...");
    ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Wi-Fi conectado com sucesso");

    print_ap_info();
    init_sntp();

    while (1) {
        time_t now;
        struct tm timeinfo;
        char buffer[64];

        time(&now);
        localtime_r(&now, &timeinfo);

        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
        ESP_LOGI(TAG, "Data/Hora Atual: %s", buffer);

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}