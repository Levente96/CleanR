#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_err.h"
#include "protocol_examples_common.h"
#include "nvs.h"
#include "nvs_flash.h"

#include <esp_http_server.h>

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/pwm.h"


static const char *TAG="APP";
#define PWM_0_OUT_IO_NUM   2
#define PWM_PERIOD    (40)
const uint32_t pin_num[1] = {
    PWM_0_OUT_IO_NUM
};
uint32_t duties[4] = {
    20, 20, 20, 20,
};
float phase[4] = {
    0, 0, 90.0, -90.0,
};

esp_err_t home_get_handler(httpd_req_t *req)
{
    //char*  buf;
    //size_t buf_len;
    
    const char* resp_str = (const char*) req->user_ctx;

    httpd_resp_send(req, resp_str, strlen(resp_str));

    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

httpd_uri_t home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = home_get_handler,
    .user_ctx  = "\
    		  <html>\
		  <head>\
	          <title>CleanR 1.0</title>\
		  </head>\
		  <body style=\"font-family: Arial; background-color: #263238;\">\
		  <center><h1 style=\"padding: 24pt; font-size: 96pt; font-family: Arial;color: #E1F5FE;\">Clean<span style=\"color: #039BE5;\">R</span></h1></center>\
	          <form action=\"/setup\" method=\"post\">\
		  <center>\
		  <img src=\"https://www.freepnglogos.com/uploads/fan-png/fan-svg-png-icon-download-onlinewebfontsm-31.png\" width=\"128\" alt=\"fan svg png icon download onlinewebfontsm\" /></br>\
		  <label style=\"font-size: 20pt; color: #E1F5FE;\" for=\"percent\">Set fan speed to:</br><span style=\"font-size:8pt; color: #607D8B; \">(0-100)</span></label><br>\
		  <input style=\"color: #0277BD; font-weight: 900; background-color: #4b4b4b; border: none; text-align: center; font-size: 36pt; padding: 6pt; margin: 24pt;\" type=\"number\" id=\"percent\" name=\"percent\" value=\"50\" min=\"0\" max=\"100\"><br>\
		  <input style=\"border-radius: 15px; background-color: #616161; color: #43A047; border: none; padding: 18pt; padding-top: 6pt; padding-bottom:6pt;  font-size: 24pt; margin: 6pt;\" type=\"submit\" value=\"Set\">\
		  </center>\
		  </form>\
		  </body>\
		  </html>"
};

esp_err_t setup_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    while (remaining > 0) {
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            return ESP_FAIL;
        }
        
        /*
        pwm_init(PWM_PERIOD, duties, 1, pin_num);
	pwm_set_phases(phase);
	pwm_start();
        */

        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
        ESP_LOGI(TAG, "%.*s", ret, buf);
        ESP_LOGI(TAG, "====================================");
    }

     const char resp[] = "<head> \
			   <meta http-equiv=\"refresh\" content=\"1; URL=/\" />	\
			   </head>";
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

httpd_uri_t setup = {
    .uri       = "/setup",
    .method    = HTTP_POST,
    .handler   = setup_post_handler,
    .user_ctx  = NULL
};


httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &home);
        httpd_register_uri_handler(server, &setup);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

static httpd_handle_t server = NULL;

static void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());


    ESP_ERROR_CHECK(example_connect());

    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

    server = start_webserver();
}
