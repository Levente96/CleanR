#ifndef CLEANR_HTTP_SERVER
#define CLEANR_HTTP_SERVER

#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_err.h"
#include <esp_http_server.h>

#include "pwm_service.h"

extern const uint8_t server_index_html_start[] asm("_binary_index_html_start");
extern const uint8_t server_index_html_end[] asm("_binary_index_html_end");

esp_err_t home_get_handler(httpd_req_t *req);

int char2int (char *array, size_t n);

esp_err_t setup_post_handler(httpd_req_t *req);

httpd_handle_t start_webserver(void);

void stop_webserver(httpd_handle_t server);

void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void connect_handler(void* arg, esp_event_base_t event_base,  int32_t event_id, void* event_data);

#endif //CLEANR_HTTP_SERVER
