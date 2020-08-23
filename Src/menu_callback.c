/*
 * menu_callback.c
 *
 *  Created on: 31 maj 2020
 *      Author: Arek
 */

#include <stdbool.h>
#include <stdio.h>  //sprintf()
#include "menu_callback.h"
#include "common.h"
#include "main.h"
#include "st7735s_spi_lib.h"
#include "clock.h"
#include "menu.h"
#include "app.h"
#include "backup_memory.h"


extern const unsigned char font1[];
extern const unsigned char font2[];

extern void (*key_up_func)(void);
extern void (*key_down_func)(void);
extern void (*key_enter_func)(void);
extern void (*key_back_func)(void);


// variable to menu_callback
static bool actualization           = true;
static bool actualizationOption0    = true;
static bool actualizationOption1    = true;
static bool breakLoop               = true;
static int8_t numberOfOption           = 0;

static uint8_t clockHours = 0;
static uint8_t clockMinutes = 0;


// function to support callback functions
static void break_loop();
static void next_option();
static void back_to_menu();

// functions for set temperature
static void add_night_temperature();
static void sub_night_temperature();
static void add_day_temperature();
static void sub_day_temperature();

// functions for set humidity
static void add_humidity();
static void sub_humidity();

// functions for set the clock
static void add_clock_minutes();
static void sub_clock_minutes();
static void add_clock_hours();
static void sub_clock_hours();


void callback_temperature()
{
  key_up_func     = add_day_temperature;
  key_down_func   = sub_day_temperature;
  key_enter_func  = next_option;
  key_back_func   = break_loop;
  while(breakLoop){
    key_press_up();
    key_press_down();
    key_press_enter();
    key_press_back();

    if(numberOfOption == 1){
      key_up_func   = add_night_temperature;
      key_down_func = sub_night_temperature;
    }

    if(actualization){  // first function call
      //set day temperature
      ST7735_ClearScreen(RGB(0, 0, 0));
      ST7735_FillRect(0, 0, 127, 16, RGB(128, 128, 128));
      ST7735_WriteTextXY("TEMPERATURA\0", 20, 1, RGB(255, 255, 255),
                          RGB(128, 128, 128), font2);

      ST7735_FillRect(0, 12 + 5, 127, 12 + 5 + 11, RGB(0, 70, 0));
      ST7735_WriteTextXY("Temperatura w dzien", 5, 12 + 7, RGB(255, 255, 255),
                          RGB(0, 70, 0), font1);

      ST7735_FillRect(0, 2*12 + 5 + 17, 127, 2*12 + 5 + 17 + 11, RGB(0, 70, 0));
      ST7735_WriteTextXY("Temperatura w nocy", 5, 12 + 7 + 11 + 17, RGB(255, 255, 255),
                          RGB(0, 70, 0), font1);

      actualization = false;
    }

    if(actualizationOption1){
      ST7735_FillRect(0, 2*12 + 5, 127, 12 + 5 + 11 + 17, RGB(0, 70, 0));
      char buffTempDay[5];
      sprintf(buffTempDay, "%02d C", get_day_temperature_value());
      ST7735_WriteTextXY(buffTempDay,  20, 12 + 7 + 11, RGB(255, 255, 255),
                          RGB(0, 70, 0), font2);
      ST7735_DrawCircle(40, 12 + 7 + 11 + 4, 2, RGB(255, 255, 255));

      actualizationOption1 = false;
    }

    //set night temperature
    if(actualizationOption0){
      ST7735_FillRect(0,12 + 5 + 17 + 2*11, 127, 3*12 + 5 + 17 + 17, RGB(0, 70, 0));
      char buffTempNight[5];
      sprintf(buffTempNight, "%02d C", get_night_temperature_value());
      ST7735_WriteTextXY(buffTempNight, 20, 12 + 7 + 11 + 17 + 12, RGB(255, 255, 255),
                         RGB(0, 70, 0), font2);
      ST7735_DrawCircle(40,  12 + 7 + 11 + 17 + 12 + 4, 2, RGB(255, 255, 255));

      actualizationOption0 = false;
    }
  } //  end main loop function
  // write changes to backup memory
  write_day_temp_to_BR((uint8_t)get_day_temperature_value());
  write_night_temp_to_BR((uint8_t)get_night_temperature_value());
  back_to_menu();
}

void callback_humidity()
{
  key_up_func     = add_humidity;
  key_down_func   = sub_humidity;
  key_enter_func  = NULL;
  key_back_func   = break_loop;
  while(breakLoop){
    key_press_up();
    key_press_down();
    key_press_enter();
    key_press_back();

    if(actualization){  // first function call
      ST7735_ClearScreen(RGB(0, 0, 0));
      ST7735_FillRect(0, 0, 127, 16, RGB(128, 128, 128));
      ST7735_WriteTextXY("WILGOTNOSC\0", 16, 1, RGB(255, 255, 255),
                          RGB(128, 128, 128), font2);

      ST7735_FillRect(0, 12 + 5, 127, 12 + 5 + 11, RGB(0, 70, 0));
      ST7735_WriteTextXY("Ustaw wilgotnosc", 5, 12 + 7, RGB(255, 255, 255),
                          RGB(0, 70, 0), font1);
      actualization = false;
    }

    //set humidity
    if(actualizationOption0){
      ST7735_FillRect(0, 2*12 + 5, 127, 12 + 5 + 11 + 17, RGB(0, 70, 0));
      char buffHumidity[5];
      sprintf((char*)buffHumidity, "%02d%c", get_humidity_value(), '%');
      ST7735_WriteTextXY(buffHumidity, 20, 12 + 7 + 11, RGB(255, 255, 255),
                         RGB(0, 70, 0), font2);

    actualizationOption0 = false;
    }
  } //  end main loop function

  // write changes to backup memory
  write_humidity_to_BR((uint8_t)get_humidity_value());
  back_to_menu();
}

void callback_daily_cycle_yes() // not finished
{
  //   static RTC_TimeTypeDef t1 = {6, 30, 0};
  //   static RTC_TimeTypeDef t2 = {21, 0, 0};
  RTC_TimeTypeDef t1 = {6, 30, 0};
  RTC_TimeTypeDef t2 = {21, 0, 0};

  // static bool visitSetNight = false;
  // static bool visitSetDay   = false;
  bool visitSetNight = false;
  bool visitSetDay   = false;

  key_up_func     = add_clock_hours;
  key_down_func   = sub_clock_hours;
  key_enter_func  = next_option;
  key_back_func   = break_loop;

  while(breakLoop){
    key_press_up();
    key_press_down();
    key_press_enter();
    key_press_back();

    if(numberOfOption == 1 || numberOfOption == 3){
      key_up_func   = add_clock_minutes;
      key_down_func = sub_clock_minutes;
    }
    else if(numberOfOption == 0 || numberOfOption == 2){
      key_up_func = add_clock_hours;
      key_down_func = sub_clock_hours;
    }

    if(actualization){  // first function call
      ST7735_ClearScreen(RGB(0, 0, 0));
      ST7735_FillRect(0, 0, 127, 16, RGB(128, 128, 128));
      ST7735_WriteTextXY("CYKL DOBOWY\0", 20, 1, RGB(255, 255, 255),
                          RGB(128, 128, 128), font2);

      ST7735_FillRect(0, 12 + 5, 127, 12 + 5 + 11, RGB(0, 70, 0));
      ST7735_WriteTextXY("Rozpoczecie dnia", 5, 12 + 7, RGB(255, 255, 255),
                          RGB(0, 70, 0), font1);

      ST7735_FillRect(0, 2*12 + 5, 127, 12 + 5 + 11 + 17, RGB(0, 70, 0));
      char buffClock[8];
      sprintf(buffClock, "%02d:%02d", t1.Hours, t1.Minutes);
      ST7735_WriteTextXY(buffClock,  20, 12 + 7 + 11, RGB(255, 255, 255),
                          RGB(0, 70, 0), font2);

      ST7735_FillRect(0, 2*12 + 5 + 17, 127, 2*12 + 5 + 17 + 11, RGB(0, 70, 0));
      ST7735_WriteTextXY("Zakonczenie dnia", 5, 12 + 7 + 11 + 17, RGB(255, 255, 255),
                          RGB(0, 70, 0), font1);

      ST7735_FillRect(0,12 + 5 + 17 + 2*11, 127, 3*12 + 5 + 17 + 17, RGB(0, 70, 0));
      sprintf(buffClock, "%02d:%02d", t2.Hours, t2.Minutes);
      ST7735_WriteTextXY(buffClock, 20, 12 + 7 + 11 + 17 + 12, RGB(255, 255, 255),
                          RGB(0, 70, 0), font2);

      actualization = false;
    }

    if (numberOfOption == 0 || numberOfOption == 1) { // set start day
      if(actualizationOption0){
        if(!visitSetDay){
          visitSetDay   = true;
          clockHours    = t1.Hours;
          clockMinutes  = t1.Minutes;
        }
        ST7735_FillRect(0, 2*12 + 5, 127, 12 + 5 + 11 + 17, RGB(0, 70, 0));
        char buffClock[8];
        sprintf(buffClock, "%02d:%02d", clockHours, clockMinutes);
        ST7735_WriteTextXY(buffClock,  20, 12 + 7 + 11, RGB(255, 255, 255),
                            RGB(0, 70, 0), font2);

        actualizationOption0 = false;
      }
      t1.Hours    = clockHours;
      t1.Minutes  = clockMinutes;
    }
    else if(numberOfOption > 1){  // set finish day
      if(actualizationOption0 || !visitSetNight){
        if(!visitSetNight){
          visitSetNight = true;
          clockHours    = t2.Hours;
          clockMinutes  = t2.Minutes;
        }

        ST7735_FillRect(0,12 + 5 + 17 + 2*11, 127, 3*12 + 5 + 17 + 17, RGB(0, 70, 0));
        char buffClock[8];
        sprintf(buffClock, "%02d:%02d", clockHours, clockMinutes);
        ST7735_WriteTextXY(buffClock, 20, 12 + 7 + 11 + 17 + 12, RGB(255, 255, 255),
                           RGB(0, 70, 0), font2);

        actualizationOption0 = false;
      }
      t2.Hours    = clockHours;
      t2.Minutes  = clockMinutes;
    }

  }//  end main loop function
  visitSetDay   = false;
  visitSetNight = false;
  set_daily_cycle(t1, t2);

  // write changes to backup memory
  write_start_day_to_BR(&t1);
  write_stop_day_to_BR(&t2);
  
  back_to_menu();
}

void callback_daily_cycle_no()
{
  RTC_TimeTypeDef t = {0, 0, 0};
  set_daily_cycle(t, t);

  // write changes to backup memory
  write_start_day_to_BR(&t);
  write_stop_day_to_BR(&t);

  menu_back();
}

void callback_lighting_yes()
{
  set_lighting(true);

  // write changes to backup memory
  write_lighting_to_BR(true);

  menu_back();
}

void callback_lighting_no()
{
  set_lighting(false);

  // write changes to backup memory
  write_lighting_to_BR(false);

  menu_back();
}


void callback_clock()
{
  RTC_TimeTypeDef t;
  const  RTC_TimeTypeDef* buff;
  buff            = get_time_struct();
  clockHours      = buff->Hours;
  clockMinutes    = buff->Minutes;

  key_up_func     = add_clock_hours;
  key_down_func   = sub_clock_hours;
  key_enter_func  = next_option;
  key_back_func   = break_loop;
  while(breakLoop){
    key_press_up();
    key_press_down();
    key_press_enter();
    key_press_back();

    if(numberOfOption == 1){
      key_up_func   = add_clock_minutes;
      key_down_func = sub_clock_minutes;
    }

    if(actualization){  // first function call
      ST7735_ClearScreen(RGB(0, 0, 0));
      ST7735_FillRect(0, 0, 127, 16, RGB(128, 128, 128));
      ST7735_WriteTextXY("ZEGAR\0", 44, 1, RGB(255, 255, 255),
                          RGB(128, 128, 128), font2);

      actualization = false;
    }

    if(actualizationOption0){
      ST7735_FillRect(0, 12 + 5, 127, 12 + 5 + 17, RGB(0, 70, 0));
      char buffClock[8];
      sprintf(buffClock, "%02d:%02d", clockHours, clockMinutes);
      ST7735_WriteTextXY(buffClock, 44, 12 + 7, RGB(255, 255, 255), RGB(0, 70, 0), font2);

      actualizationOption0 = false;
    }

  } //  end main loop function
  t.Hours   = (uint8_t)clockHours;
  t.Minutes = (uint8_t)clockMinutes;
  t.Seconds = 0;
  set_clock(&t);
  back_to_menu();

}

void callback_exit()
{
  menu_exit_function();
  back_menu();
}



// functions for set temperature
static void add_night_temperature()
{
  int temp = get_night_temperature_value();
  if(temp < 35)
    set_night_temperature(temp+1);

  actualizationOption0 = true;
}

static void sub_night_temperature()
{
  int temp = get_night_temperature_value();
  if(temp > 5)
    set_night_temperature(temp-1);

  actualizationOption0 = true;
}

static void add_day_temperature()
{
  int temp = get_day_temperature_value();
  if(temp < 35)
    set_day_temperature(temp+1);

  actualizationOption1 = true;
}

static void sub_day_temperature()
{
  int temp = get_day_temperature_value();
    if(temp > 5)
      set_day_temperature(temp-1);

    actualizationOption1 = true;
}
//-----------------------------------------------------------------------------

// functions for set humidity
static void add_humidity()
{
  int temp = get_humidity_value();
  if(temp < 80)
    set_humidity(temp+1);
  actualizationOption0 = true;
}

static void sub_humidity()
{
  int temp = get_humidity_value();
    if(temp > 0)
      set_humidity(temp-1);
    actualizationOption0 = true;
}

//-----------------------------------------------------------------------------

// functions for set the clock

static void add_clock_minutes()
{
  clockMinutes++;
  if(clockMinutes >= 60){
    clockMinutes = 0;
  }
  actualizationOption0 = true;
}

static void sub_clock_minutes()
{
  clockMinutes--;
  if(clockMinutes <= 0){
    clockMinutes = 59;
  }
  actualizationOption0 = true;
}

static void add_clock_hours()
{
  clockHours++;
  if(clockHours >= 24){
    clockHours = 0;
  }
  actualizationOption0 = true;
}

static void sub_clock_hours()
{
  clockHours--;
  if(clockHours <= 0){
    clockHours = 23;
  }
  actualizationOption0 = true;
}

//-----------------------------------------------------------------------------
// function to support callback functions
static void back_to_menu()
{
  key_up_func     = menu_next;
  key_down_func   = menu_prev;
  key_enter_func  = menu_enter;
  key_back_func   = menu_back;
  breakLoop       = true;
  numberOfOption  = 0;
  actualization   = true;
  actualizationOption1 = true;
  actualizationOption0 = true;
  menu_back();
}

static void break_loop()  { breakLoop = false; }
static void next_option() { numberOfOption++; }
