/*
 * common.h
 *
 *  Created on: May 29, 2020
 *      Author: arek
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "main.h"

typedef enum {day = 0, night = 1} dailyCycle;

void (*key_up_func)(void);
void (*key_down_func)(void);
void (*key_enter_func)(void);
void (*key_back_func)(void);


int get_day_temperature_value();
int get_night_temperature_value();
int get_humidity_value();
bool get_lighting_value();
int get_temperature_from_sensor();
int get_humidity_from_sensor();


//set values form user to terrarium control
void set_day_temperature(int _temperature);
void set_night_temperature(int _temperature);
void set_humidity(int _humidity);
void set_lighting(bool _lighting);
void set_daily_cycle(RTC_TimeTypeDef _startDay, RTC_TimeTypeDef _stopDay);
dailyCycle get_daily_cycle();


//terrarium control
void heating_control();
void humidity_control();
void lighting_control();

//read values from sensors
int get_temp_from_sensor();
int get_humid_from_sensor();


//functions for buttons
void key_press_up();
void key_press_down();
void key_press_enter();
void key_press_back();

#endif /* COMMON_H_ */
