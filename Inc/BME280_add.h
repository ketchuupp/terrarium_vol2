/*
 * bme280_add.h
 *
 *  Created on: Nov 29, 2019
 *      Author: piotr
 */

#ifndef INC_BME280_ADD_H_
#define INC_BME280_ADD_H_

int8_t BME280_init(void);
int8_t BME280_read_data(void);
uint32_t BME_read_pressure(void);
uint32_t BME_read_humidity(void);
int32_t BME_read_temp(void);
void print_sensor_data(struct bme280_data *comp_data);
void delay_ms(uint32_t period);
int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);


#endif /* INC_BME280_ADD_H_ */
