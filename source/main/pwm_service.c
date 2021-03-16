#include "pwm_service.h"

const uint32_t pwm_pin_num[1] = { PWM_FAN_OUT_IO_NUM };
uint32_t pwm_duty_cycle_us[1] = { 23 };
float pwm_phase_shift[1] = { 0 };
uint8_t pwm_fan_speed = 0;

void pwm_init_service()
{
	pwm_init(PWM_PERIOD_US, pwm_duty_cycle_us, 1, pwm_pin_num);
	pwm_set_phases(pwm_phase_shift);
	pwm_start();
	pwm_fan_speed = (uint8_t)((float)(pwm_duty_cycle_us[0] / PWM_PERIOD_US) * 100);
}

void pwm_set_fan_speed(uint8_t percent)
{
	if(percent > 100) percent = 100;
	
	pwm_duty_cycle_us[0] = (uint8_t)((float)(PWM_PERIOD_US / 100.0) * percent);
	pwm_fan_speed = percent;
	pwm_set_duties(pwm_duty_cycle_us);
}

void pwm_get_fan_speed(uint8_t* percent)
{
	//TODO
}