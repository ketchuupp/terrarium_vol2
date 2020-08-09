/*
 * menu.c
 *
 *  Created on: May 30, 2020
 *      Author: arek
 */

#include <stdbool.h>
#include "main.h"
#include "st7735s_spi_lib.h"
#include "font1.h"
#include "font2.h"
#include "menu.h"
#include "menu_callback.h"
#include "app.h"


menu_t temperature =  {"Temperatura", &humidity, &menu_exit, NULL, NULL,
                        callback_temperature };
menu_t humidity = 		{"Wilgotnosc", &day_time, &temperature, NULL, NULL,
                        callback_humidity};
menu_t day_time = 		{"Cykl dobowy",&lighting, &humidity, &day_time_yes, NULL, NULL};
	menu_t day_time_yes = 	{"Wlacz cykl dobowy", &day_time_no, &day_time_no, NULL,
														&day_time, callback_daily_cycle_yes};
	menu_t day_time_no = 		{"Wylacz cykl dobowy", &day_time_yes, &day_time_yes, NULL,
														&day_time, callback_daily_cycle_no};
menu_t lighting = 		{"Oswietlenie", &clock, &day_time, &lighting_yes, NULL, NULL};
	menu_t lighting_yes = 	{"Oswietlenie ON", &lighting_no, &lighting_no, NULL,
														&lighting, callback_lighting_yes	};
	menu_t lighting_no =  	{"Oswietlenie OFF", &lighting_yes, &lighting_yes, NULL,
														&lighting, callback_lighting_no	};
menu_t clock = 				{"Zegar", &menu_exit, &lighting, NULL, NULL, callback_clock};
menu_t menu_exit = 		{"Wyjscie", &temperature, &clock, NULL, NULL, callback_exit};



menu_t *currentPointer = &temperature;
signed char actualization = 0;
signed char position = 1;


static uint8_t menu_get_index(menu_t *q)
{
	menu_t *temp;
	if (q->parent)
	  temp = (q->parent)->child;
	else
	  temp = &temperature;

	int i = 1;
	while (temp != q){
		temp = temp->next;
		i++;
	}


	return i;
}

/*
static uint8_t menu_get_level(menu_t *q)
{

	menu_t *temp = q;
	uint8_t i = 0;

	if(!q->parent)
	  return 0;

	while(temp->parent != NULL){
		temp = temp->parent;
		i++;
	}
	return i;
}
*/


static uint8_t menu_get_amount(menu_t *q)
{
	menu_t *temp = q->next;
	uint8_t i = 1;

	while(temp != q){
		temp = temp->next;
		i++;
	}
	return i;
}

void menu_refresh(void)
{
	menu_t *temp;
	if(currentPointer->parent)
		temp = (currentPointer->parent)->child;
	else
	  temp = &temperature;



	while (actualization >= 0){

		if(actualization == 0){
			ST7735_ClearScreen(RGB(0, 0, 0));
			ST7735_FillRect(0, 0, 127, 16, RGB(128, 128, 128));
			ST7735_WriteTextXY("MENU\0", 48, 1, RGB(255, 255, 255),
					RGB(128, 128, 128), font2);
		}

		uint8_t amount_options = menu_get_amount(currentPointer);
		//drawing menu inscriptions
		for(int i = 1; i <= amount_options; i++){
		  //only last position
		  //or the whole menu if it is refreshed
			if((i == actualization) || (actualization == 0)) {
				ST7735_FillRect(0, i * 12 + 5, 127, i * 12 + 5 + 11, RGB(0, 150, 0));
				ST7735_WriteTextXY(temp->name, 5, i * 12 + 7, RGB(255, 255, 255),
						RGB(0, 150, 0), font1);
			}

			//draw current selected position
			if(i == menu_get_index(currentPointer)){
				ST7735_FillRect(0, i * 12 + 5, 127, i * 12 + 5 + 11, RGB(0, 70, 0));
				ST7735_WriteTextXY(temp->name, 5, i * 12 + 7, RGB(255, 255, 255),
						RGB(0, 70, 0), font1);
			}
			temp = temp->next;
		}
		actualization = -1;

	}
}

void menu_next(void)
{
  actualization = menu_get_index(currentPointer);
  currentPointer = currentPointer->prev;
  menu_refresh();

}

void menu_prev(void)
{
  actualization = menu_get_index(currentPointer);
  currentPointer = currentPointer->next;
  menu_refresh();
}

void menu_enter(void)
{
	if(currentPointer->menu_function)
	  currentPointer->menu_function();

	else if(currentPointer->child){
		ST7735_ClearScreen(RGB(0, 0, 0));
		currentPointer = currentPointer->child;
		actualization = 0;
		//position = 1;
		menu_refresh();
	}

}

void menu_back(void)
{
	if(currentPointer->parent){
		ST7735_ClearScreen(RGB(0, 0, 0));
		actualization = 0;
		//position = 1;
		currentPointer = currentPointer->parent;
		menu_refresh();
	}
	else{ // back from callback function
	  actualization = 0;
	  menu_refresh();
	}
}

void menu_exit_function(void)
{
  currentPointer = &temperature;
  actualization = 0;
  position = 1;
}

