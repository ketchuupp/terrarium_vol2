/*
 * app.c
 *
 *  Created on: 10 cze 2020
 *      Author: Arek
 */

#include "app.h"

#include <stdbool.h>
#include <stdio.h>  //sprintf

//#include "stm32f1xx_hal_rcc.h"
#include "tim.h"
#include "st7735s_spi_lib.h"
#include "menu.h"
#include "clock.h"
#include "main.h"
#include "common.h"
#include "backup_memory.h"
#include "BME280_add.h"

static int interruptChangesFlag = 1;
static int backMenuFlag         = 0;
static int runMenuFlag          = 0;

extern void (*key_up_func)(void);
extern void (*key_down_func)(void);
extern void (*key_enter_func)(void);
extern void (*key_back_func)(void);

extern const unsigned char font1[];
extern const unsigned char font2[];

static void read_reset_flags(void);
static void set_parameters_from_BCKU_memory();

void app(void)
{
  read_reset_flags();

  ST7735_SPI_HwConfig();
  ST7735_SPI_Init();
  __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(&htim2);
  //BME280_init();
  //BME280_read_data();

  read_temp_from_sensor();
  read_humid_from_sensor();
  heating_control();
  humidity_control();
  lighting_control();
  read_time_from_RTC();

  key_up_func     = NULL;
  key_down_func   = NULL;
  key_enter_func  = run_menu;
  key_back_func   = NULL;

  

  while(1){ // main program loop
    key_press_enter();

    while(runMenuFlag){
      key_press_up();
      key_press_down();
      key_press_enter();
      key_press_back();
    }

    if(interruptChangesFlag || backMenuFlag){
      //draw default screen

      // draw clock
      const RTC_TimeTypeDef* t;
      t = get_time_struct();
      static uint8_t buffClock = 61;

      if (buffClock != t->Minutes || backMenuFlag) {
        ST7735_FillRect(0, 12 + 5, 127, 12 + 5 + 17, RGB(0, 0, 0));
        char buffClockTxt[8];
        sprintf(buffClockTxt, "%02d:%02d", t->Hours, t->Minutes);
        ST7735_WriteTextXY(buffClockTxt,  44, 12 + 7, RGB(255, 255, 255),
                            RGB(0, 0, 0), font2);
        buffClock = t->Minutes;
      }

      //draw temperature
      int temperature = get_temperature_from_sensor();
      static int buffTemp = -1;

      if(buffTemp != temperature || backMenuFlag){
        ST7735_FillRect(0, 12 + 5 + 15, 127, 12 + 5 + 17 + 15, RGB(0, 0, 0));
        char buffTempTxt[5];
        sprintf(buffTempTxt, "%02d C", temperature);
        ST7735_WriteTextXY(buffTempTxt,  44, 12 + 7 + 15, RGB(255, 255, 255),
                            RGB(0, 0, 0), font2);
        ST7735_DrawCircle(64, 12 + 7 + 15 + 4, 2, RGB(255, 255, 255));
        buffTemp = temperature;
      }

      // draw humidity
      int humidity = get_humidity_from_sensor();
      static int buffHumidity = -1;

      if(buffHumidity != humidity || backMenuFlag){
        ST7735_FillRect(0, 12 + 5 + 15 +15, 127, 12 + 5 + 17 + 15 + 15, RGB(0, 0, 0));
        char buffHumidTxt[4];
        sprintf(buffHumidTxt, "%02d%c", humidity, '%');
        ST7735_WriteTextXY(buffHumidTxt,  44, 12 + 7 + 15 + 15, RGB(255, 255, 255),
                            RGB(0, 0, 0), font2);
        buffHumidity = humidity;
      }
      interruptChangesFlag = 0;
      backMenuFlag = 0;
    }

  }
}

void run_menu(void)
{
  runMenuFlag = 1;
  key_up_func     = menu_next;
  key_down_func   = menu_prev;
  key_enter_func  = menu_enter;
  key_back_func   = menu_back;
  menu_refresh();
}

void back_menu(void)
{
  backMenuFlag    = 1;
  runMenuFlag     = 0;
  key_up_func     = NULL;
  key_down_func   = NULL;
  key_enter_func  = run_menu;
  key_back_func   = NULL;
  ST7735_ClearScreen(RGB(0, 0, 0));
}

// declaration in stm32f1xx_hal_tim.h
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2){
    BME280_read_data();
    read_temp_from_sensor();
    read_humid_from_sensor();

    heating_control();
    humidity_control();
    lighting_control();
    read_time_from_RTC();
    interruptChangesFlag = 1;
  }
}


static void read_reset_flags(void)
{
  // Pin reset
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)){
    set_parameters_from_BCKU_memory();
  }
  // software reset
  else if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)){
    set_parameters_from_BCKU_memory();
  }
  // Low Power reset
  else if(__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)){
    set_parameters_from_BCKU_memory();
  }
  else{
    // first device launch
  set_day_temperature(26);
  set_night_temperature(22);
  set_humidity(50);
  set_lighting(false);

  RTC_TimeTypeDef t1 = {7,0,0};
  RTC_TimeTypeDef t2 = {21,0,0};
  set_daily_cycle(t1, t2);
  }
  //IWDG_KR_KEY
  __HAL_RCC_CLEAR_RESET_FLAGS();
}

static void set_parameters_from_BCKU_memory()
{
  read_time_from_RTC();
  set_day_temperature((int)read_day_temp_from_BR());
  set_night_temperature((int)read_night_temp_from_BR());
  set_humidity((int)read_humidity_from_BR());
  set_lighting(read_lighting_from_BR());
  set_daily_cycle(read_start_day_from_BR(), read_stop_day_from_BR());
}