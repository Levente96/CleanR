#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/pwm.h"


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

void app_main()
{
    pwm_init(PWM_PERIOD, duties, 1, pin_num);
    pwm_set_phases(phase);
    pwm_start();

    while (1) {
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

