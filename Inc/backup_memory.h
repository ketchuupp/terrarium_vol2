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
#include "rtc.h"

extern RTC_HandleTypeDef hrtc;

// adresses RTC_BKP_DR2 -> RTC_BKP_DR18

// first 8-bits -> day temperature
// second 8-bits -> night temperature
#define ADDR_TEMP RTC_BKP_DR2

// first 8-bits -> lighting
// second 8-bits -> humidity
#define ADDR_LIGHTING_AND_HUMIDITY RTC_BKP_DR3

// first 8-bits -> minutes
// second 8-bits -> hours
#define ADDR_START_DAY RTC_BKP_DR4
#define ADDR_STOP_DAY RTC_BKP_DR5

#define RTC_ADDR &hrtc

void write_day_temp_to_BR(uint8_t t);
uint8_t read_day_temp_from_BR(void);

void write_night_temp_to_BR(uint8_t);
uint8_t read_night_temp_from_BR(void);

void write_humidity_to_BR(uint8_t h);
uint8_t read_humidity_from_BR(void);

void write_lighting_to_BR(bool l);
bool read_lighting_from_BR(void);

void write_start_day_to_BR(const RTC_TimeTypeDef *t);
RTC_TimeTypeDef read_start_day_from_BR(void);

void write_stop_day_to_BR(const RTC_TimeTypeDef *t);
RTC_TimeTypeDef read_stop_day_from_BR(void);

#endif /* BACKUP_MEMORY_H_*/
