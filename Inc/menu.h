/*
 * menu.h
 *
 *  Created on: May 30, 2020
 *      Author: arek
 */

#ifndef MENU_H_
#define MENU_H_

typedef struct menu_struct menu_t;

struct menu_struct{

	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);

};

menu_t temperature;
	menu_t temperature_night;
	menu_t temperature_day;
menu_t humidity;
menu_t day_time;
	menu_t day_time_yes;
	menu_t day_time_no;
menu_t lighting;
	menu_t lighting_yes;
	menu_t lighting_no;
menu_t clock;
menu_t menu_exit;

void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void menu_back(void);
void menu_exit_function(void);

#endif /* MENU_H_ */
