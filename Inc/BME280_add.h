/* 
 * BME280_add.h 
*/
#ifndef BME280_ADD_H_
#define BME280_ADD_H_

#include "bme280_defs.h"

int8_t BME280_init(void);
int8_t BME280_read_data(void);
uint32_t BME_read_pressure(void);
uint32_t BME_read_humidity(void);
int32_t BME_read_temp(void);
void delay_ms(uint32_t period, void *intf_ptr);
int8_t i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr); 
int8_t i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);


#endif /* INC_BME280_ADD_H_ */
