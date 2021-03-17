#ifndef _PWM_SERVICE_H_
#define _PWM_SERVICE_H_

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

#define PWM_FAN_OUT_IO_NUM         2
#define PWM_PERIOD_US                    46

extern const uint32_t pwm_pin_num[];
extern uint32_t pwm_duty_cycle_us[];
extern float pwm_phase_shift[];
extern uint8_t pwm_fan_speed;

void pwm_init_service();
void pwm_set_fan_speed(uint8_t percent);
void pwm_get_fan_speed(uint8_t* percent);

#endif //_PWM_SERVICE_H_