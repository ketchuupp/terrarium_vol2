/*
 * common.c
 *
 *  Created on: May 29, 2020
 *      Author: arek
 */

#include <stdbool.h>
#include "common.h"
#include "main.h"
#include "menu.h"
#include "clock.h"

static int setDayTemperature     = 25;    //C
static int setNightTemperature   = 20;    //C
static int setHumidity           = 50;    //%

static int temperatureFromSensor;
static int humidityFromSensor;

static bool setLighting          = false;
static RTC_TimeTypeDef startDay  = {6, 0, 0,};
static RTC_TimeTypeDef stopDay   = {22, 0, 0,};


// getters
int get_day_temperature_value()   { return setDayTemperature; }
int get_night_temperature_value() { return setNightTemperature; }
int get_humidity_value()          { return setHumidity; }
bool get_lighting_value()         { return setLighting; }
int get_temperature_from_sensor() { return temperatureFromSensor; }
int get_humidity_from_sensor()    { return humidityFromSensor; }



//set values by user to terrarium control
// setters
void set_day_temperature(int _temperature)    { setDayTemperature = _temperature; }
void set_night_temperature(int _temperature)  { setNightTemperature = _temperature; }
void set_humidity(int _humidity)              { setHumidity = _humidity; }
void set_lighting(bool _lighting)             { setLighting = _lighting; }
void set_daily_cycle(RTC_TimeTypeDef _startDay, RTC_TimeTypeDef _stopDay)
{
  startDay = _startDay;
  stopDay  = _stopDay;
}

enum dailyCycle get_daily_cycle()
{
  if(startDay.Hours == stopDay.Hours && startDay.Minutes == stopDay.Minutes)
    return day;

  const RTC_TimeTypeDef* t = get_time_struct();
  if((t->Hours*60+t->Minutes) >= (startDay.Hours*60+startDay.Minutes)){
        if((t->Hours*60+t->Minutes) <= (stopDay.Hours*60+stopDay.Minutes)){
            return day;
        }
      }
  return night;
}

//-----------------------------------------------------------------------------
//terrarium control
//FIXME using PWM
void heating_control() 
{
  temperatureFromSensor = get_temp_from_sensor();
  static bool heat = false;

  if(get_daily_cycle() == day){
    if(setDayTemperature-2 > temperatureFromSensor)
      heat = true;
    else if(setDayTemperature+1 < temperatureFromSensor)
      heat = false;
  }
  else{
    if(setNightTemperature-2 > temperatureFromSensor)
      heat = true;
    else if(setNightTemperature+1 < temperatureFromSensor)
      heat = false;
  }


  if(heat){
    //turn on heat
    HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, RESET);
  }
  else{
    //turn off
    HAL_GPIO_WritePin(HEAT_GPIO_Port, HEAT_Pin, SET);
  }
}

void humidity_control()
{
  humidityFromSensor = get_humid_from_sensor();
  static bool humidification = false;

  if(setHumidity-10 > humidityFromSensor)
    humidification = true;
  else if(setHumidity+10 < humidityFromSensor)
    humidification = false;

  if(humidification){
    // function to turn on the humidifier
    HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, RESET);
  }
  else{
    //turn off
    HAL_GPIO_WritePin(HUMIDIFIER_GPIO_Port, HUMIDIFIER_Pin, SET);
  }
}

void lighting_control()
{
  static bool light = 0;
  if(setLighting){
    if(get_daily_cycle() == day){
      light = true;
    }
    else{
      light = false;
    }
  }
  else{
    light = false;
  }

  if(light){
    // function to turn on the light
    HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, RESET);
  }
  else{
    //turn off
    HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, SET);
  }
}

// read values from sensors
// TODO
int get_temp_from_sensor()
{
  return 25;
}
int get_humid_from_sensor()
{
  return 39;
}

//-----------------------------------------------------------------------------
// functions for buttons

void (*key_up_func)(void)     = &menu_next;
void (*key_down_func)(void)   = &menu_prev;
void (*key_enter_func)(void)  = &menu_enter;
void (*key_back_func)(void)   = &menu_back;


void key_press_up()
{
	static uint8_t key_lock_up = 0;

	if(!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) && !key_lock_up){
		key_lock_up = 1;
		if(key_up_func)
		  (*key_up_func)();
	}
	else if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) && key_lock_up)
	  key_lock_up++;
}


void key_press_down()
{
	static uint8_t key_lock_down = 0;

	if(!HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) && !key_lock_down){
		key_lock_down = 1;
		if(key_down_func)
		  (*key_down_func)();
	}
	else if(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) && key_lock_down)
	  key_lock_down++;
}


void key_press_enter()
{
	static uint8_t key_lock_enter = 0;

	if(!HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) && !key_lock_enter){
		key_lock_enter = 1;
		if(key_enter_func)
		  (*key_enter_func)();
	}
	else if(HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) && key_lock_enter)
	  key_lock_enter++;
}


void key_press_back()
{
	static uint8_t key_lock_back = 0;

	if(!HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin) && !key_lock_back){
		key_lock_back = 1;
		if(key_back_func)
		  (*key_back_func)();
	}
	else if(HAL_GPIO_ReadPin(B4_GPIO_Port, B4_Pin) && key_lock_back)
	  key_lock_back++;
}

//-----------------------------------------------------------------------------