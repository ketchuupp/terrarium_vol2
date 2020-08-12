/*
 * back_up_memory.c
 *
 *  Created on: 09 sie 2020
 *      Author: Arek
 */

#include "main.h"
#include "../Inc/backup_memory.h"



bool write_day_temp_to_BR(uint8_t t)
{
  HAL_RTCEx_BKUPWrite(RTC_ADDR , ADDR_TEMP, t);
}
uint8_t read_day_temp_from_BR(void)
{
  unsigned int buff;
  buff = (HAL_RTCEx_BKUPRead(RTC_ADDR, ADDR_TEMP));
  return (uint8_t)buff;
}

bool write_night_temp_to_BR(uint8_t t)
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

bool write_humidity_to_BR(uint8_t);
uint8_t read_humidity_from_BR(void);

bool write_lighting_to_BR(uint8_t);
bool read_lighting_from_BR(void);

bool write_start_day_to_BR(RTC_TimeTypeDef t);
RTC_TimeTypeDef read_start_day_from_BR(void);

bool write_stop_day_to_BR(RTC_TimeTypeDef t);
RTC_TimeTypeDef read_stop_day_from_BR(void);


