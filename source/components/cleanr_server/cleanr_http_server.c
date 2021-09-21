#include "cleanr_http_server.h"


static httpd_uri_t home = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = home_get_handler,
    .user_ctx  = (char *)server_index_html_start
};

static httpd_uri_t setup = {
    .uri       = "/setup",
    .method    = HTTP_POST,
    .handler   = setup_post_handler,
    .user_ctx  = NULL
};

esp_err_t home_get_handler(httpd_req_t *req)
{
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI("Server_Service", "Request headers lost");
    }
    return ESP_OK;
}

// convert character array to integer 
int char2int (char *array, size_t n)
{    
    int number = 0;
    int mult = 1;

    n = (int)n < 0 ? -n : n; 

    while (n--)
    {
        // if not digit or '-', check if number > 0, break or continue 
        if ((array[n] < '0' || array[n] > '9') && array[n] != '-') {
            if (number)
                break;
            else
                continue;
        }

        if (array[n] == '-') {      // if '-' if number, negate, break 
            if (number) {
                number = -number;
                break;
            }
        }
        else {                      // convert digit to numeric value   
            number += (array[n] - '0') * mult;
            mult *= 10;
        }
    }

    return number;
}

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
		httpd_resp_send_chunk(req, buf, ret);
        ESP_LOGI("Server_Service", "=========== RECEIVED DATA ==========");
		uint8_t fspeed = char2int(buf, 100);
		pwm_set_fan_speed(fspeed);
		ESP_LOGI("Server_Service", "Set fan to: %d", fspeed);
        ESP_LOGI("Server_Service", "====================================");
    }
	httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI("Server_Service", "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI("Server_Service", "Registering URI handlers");
        httpd_register_uri_handler(server, &home);
        httpd_register_uri_handler(server, &setup);
        return server;
    }

    ESP_LOGI("Server_Service", "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI("Server_Service", "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

void connect_handler(void* arg, esp_event_base_t event_base,  int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI("Server_Service", "Starting webserver");
        *server = start_webserver();
    }
}
