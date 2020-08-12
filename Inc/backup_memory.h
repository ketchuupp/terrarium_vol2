/*
 * back_up_memory.h
 *
 *  Created on: 09 sie 2020
 *      Author: Arek
 */

#ifndef BACKUP_MEMORY_H_
#define BACKUP_MEMORY_H_

#include <stdbool.h>
#include  "main.h" 
#include "../Src/rtc.c"

extern RTC_HandleTypeDef hrtc;

// adresses RTC_BKP_DR2 -> RTC_BKP_DR18

// first 8-bits -> day temperature
// second 8-bits -> night temperature
#define ADDR_TEMP RTC_BKP_DR2

// first 8-bits -> lighting
// second 8-bits -> humidity
#define ADDR_LIGHTING_AND_HUMIDITY RTC_BKP_DR3

#define ADDR_START_DAY RTC_BKP_DR4
#define ADDR_STOP_DAY RTC_BKP_DR5

#define RTC_ADDR &hrtc



//void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data);
//HAL_RTCEx_BKUPRead

bool write_day_temp_to_BR(uint8_t t);
uint8_t read_day_temp_to_BR(void);

bool write_night_temp_to_BR(uint8_t);
uint8_t read_night_temp_to_BR(void);

bool write_humidity_to_BR(uint8_t);
uint8_t read_humidity_to_BR(void);

bool write_start_day_to_BR(RTC_TimeTypeDef t);
RTC_TimeTypeDef read_start_day_to_BR(void);

bool write_lighting_to_BR(uint8_t);
uint8_t read_lighting_to_BR(void);

bool write_stop_day_to_BR(RTC_TimeTypeDef t);
RTC_TimeTypeDef read_stop_day_to_BR(void);


#endif /* BACKUP_MEMORY_H_*/
