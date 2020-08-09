/*
 * clock.h
 *
 *  Created on: Jun 3, 2020
 *      Author: Arek
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "main.h"

const RTC_TimeTypeDef* get_time_struct();
void set_clock(RTC_TimeTypeDef *t);
void get_time_text(char *t);

//use in interrupt
void read_time_from_RTC();


#endif /* CLOCK_H_ */
