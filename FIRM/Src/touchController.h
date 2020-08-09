#ifndef __CAPSENSECONTROL_H
#define __CAPSENSECONTROL_H
#include "main.h"
#include "tsc.h"


#define TOUCH_NONE 0
#define TOUCH_DETECTED 1

#define TSC_UP 0
#define TSC_DOWN 1
#define TSC_LEFT 2
#define TSC_RIGHT 3
#define TSC_BTN_A 4
#define TSC_BTN_B 5

void tsc_capsense_init(void);
void CapSense_run_LPF_10ms(void);
uint8_t tsc_get_capsense(uint8_t sensor_id);
uint32_t tsc_get_capsense_raw(uint8_t sensor_id);
uint32_t tsc_get_capsense_threshold_raw(uint8_t sensor_id);

#endif
