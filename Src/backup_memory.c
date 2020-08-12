/*
 * back_up_memory.c
 *
 *  Created on: 09 sie 2020
 *      Author: Arek
 */

#include "main.h"
// #include "../Inc/backup_memory.h"
#include "backup_memory.h"



void write_day_temp_to_BR(uint8_t t)
{
  HAL_RTCEx_BKUPWrite(RTC_ADDR , ADDR_TEMP, t);
}
uint8_t read_day_temp_from_BR(void)
{
  unsigned int buff;
  buff = (HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_TEMP));
  return (uint8_t)buff;
}

void write_night_temp_to_BR(uint8_t t)
{
  HAL_RTCEx_BKUPWrite(RTC_ADDR , ADDR_TEMP, t << 8);
}
uint8_t read_night_temp_from_BR(void)
{
  uint32_t buff;
  buff = (HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_TEMP));
  return (uint8_t)(buff >> 8);
}

//--------------------------------------------------------

void write_humidity_to_BR(uint8_t h)
{
  HAL_RTCEx_BKUPWrite(RTC_ADDR , ADDR_LIGHTING_AND_HUMIDITY, h);
}
uint8_t read_humidity_from_BR(void)
{
  unsigned int buff;
  buff = (HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_LIGHTING_AND_HUMIDITY));
  return (uint8_t)buff;
}

void write_lighting_to_BR(bool l)
{
  HAL_RTCEx_BKUPWrite(RTC_ADDR , ADDR_LIGHTING_AND_HUMIDITY, (uint8_t)l << 8);
}
bool read_lighting_from_BR(void)
{
  uint32_t buff;
  buff = HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_LIGHTING_AND_HUMIDITY);
  if(buff >> 8 == 0)
    return false;
  else 
    return true;
}

void write_start_day_to_BR(const RTC_TimeTypeDef *t)
{
  uint32_t buff;
  buff = t->Minutes;
  buff += t->Hours << 8;
  HAL_RTCEx_BKUPWrite(RTC_ADDR, ADDR_START_DAY, buff);
}
RTC_TimeTypeDef read_start_day_from_BR(void)
{
  uint32_t buff = HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_START_DAY);
  RTC_TimeTypeDef t;
  t.Minutes = buff;
  t.Hours = buff >> 8;
  return t;
}

void write_stop_day_to_BR(const RTC_TimeTypeDef *t)
{
  uint32_t buff;
  buff = t->Minutes;
  buff += t->Hours << 8;
  HAL_RTCEx_BKUPWrite(RTC_ADDR, ADDR_STOP_DAY, buff);
}
RTC_TimeTypeDef read_stop_day_from_BR(void)
{
  uint32_t buff = HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_STOP_DAY);
  RTC_TimeTypeDef t;
  t.Minutes = buff;
  t.Hours = buff >> 8;
  return t;
}


