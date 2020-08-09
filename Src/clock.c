/*
 * clock.c
 *
 *  Created on: Jun 3, 2020
 *      Author: Arek
 */

#include <stdio.h>
#include "clock.h"
//#include "font2.h"

extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef Time = {21, 30, 0};

const RTC_TimeTypeDef* get_time_struct()
{
  return &Time;
}

void set_clock(RTC_TimeTypeDef *t)
{
  if(HAL_RTC_SetTime(&hrtc, t, RTC_FORMAT_BIN) != HAL_OK){
    Error_Handler();
  }
}

// array char t - size 14
void get_time_text(char *t)
{
  sprintf(t, "%3i:%02i",
      Time.Hours, Time.Minutes);
}

void read_time_from_RTC()
{
  HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
}



