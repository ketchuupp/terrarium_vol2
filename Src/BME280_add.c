/* 
 * BME280_add.c 
*/
#include <stdio.h>

#include "bme280_add.h"
#include "bme280.h"
#include "i2c.h"

static struct bme280_dev bme;
static struct bme280_data comp_data;
uint8_t dev_addr = BME280_I2C_ADDR_PRIM;


int8_t BME280_init(void) {
	int8_t rslt = BME280_OK;
	uint8_t settings_sel;

	bme.chip_id = (BME280_I2C_ADDR_PRIM<<1);
	bme.intf = BME280_I2C_INTF;
	bme.read = i2c_read;
	bme.write = i2c_write;
	bme.delay_us = delay_ms;
	bme.intf_ptr = &dev_addr;

	rslt = bme280_init(&bme);

	/* Recommended mode of operation: Indoor navigation */
	bme.settings.osr_h = BME280_OVERSAMPLING_1X;
	bme.settings.osr_p = BME280_OVERSAMPLING_16X;
	bme.settings.osr_t = BME280_OVERSAMPLING_2X;
	bme.settings.filter = BME280_FILTER_COEFF_16;
	bme.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	rslt = bme280_set_sensor_settings(settings_sel, &bme);
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme);

	return rslt;
}

/* read data in normal mode */
int8_t BME280_read_data(void) {
	int8_t rslt;

	rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme);
	//bme.delay_ms(70);

	return rslt;
}
#ifdef BME280_FLOAT_ENABLE
float BME_read_pressure(void) {
	return comp_data.pressure;
}
#else
uint32_t BME_read_pressure(void) {
	return comp_data.pressure;
}
#endif

#ifdef BME280_FLOAT_ENABLE
float BME_read_humidity(void) {
	return comp_data.humidity;
}
#else
uint32_t BME_read_humidity(void) {
	return comp_data.humidity;
}
#endif

#ifdef BME280_FLOAT_ENABLE
float BME_read_temp(void) {
	return comp_data.temperature;
}
#else
int32_t BME_read_temp(void) {
	return comp_data.temperature;
}
#endif

// FIXME
void delay_ms(uint32_t period, void *intf_ptr) {
	HAL_Delay(period);
}

int8_t i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
		uint8_t id;
    id = *((uint8_t *)intf_ptr);
		
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    HAL_I2C_Mem_Read(&hi2c2, id, reg_addr, 1, reg_data, len, 100);
    return rslt;
}

int8_t i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
		uint8_t id;
    id = *((uint8_t *)intf_ptr);
		uint8_t data = *reg_data;

    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    HAL_I2C_Mem_Write(&hi2c2, id, reg_addr, 1, &data, len, 100);
    return rslt;
}
