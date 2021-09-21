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

// TODO: Move to unified location
static const char *TAG="CleanR";

static httpd_handle_t server = NULL;

esp_err_t home_get_handler(httpd_req_t *req);

static httpd_uri_t home = {
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
		<input style=\"color: #039BE5; width: 25vw;\" type=\"range\" id=\"percent\" name=\"percent\" value=\"50\" min=\"1\" max=\"100\"><br>\
		<input style=\"border-radius: 15px; background-color: #616161; color: #43A047; border: none; padding: 18pt; padding-top: 6pt; padding-bottom:6pt;  font-size: 24pt; margin: 6pt;\" type=\"submit\" value=\"Set\">\
	</center>\
	</form>\
</body>\
</html>"
};

int char2int (char *array, size_t n);

esp_err_t setup_post_handler(httpd_req_t *req);

static httpd_uri_t setup = {
    .uri       = "/setup",
    .method    = HTTP_POST,
    .handler   = setup_post_handler,
    .user_ctx  = NULL
};

httpd_handle_t start_webserver(void);

void stop_webserver(httpd_handle_t server);

void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void connect_handler(void* arg, esp_event_base_t event_base,  int32_t event_id, void* event_data);

#endif //CLEANR_HTTP_SERVER
