#ifndef INC_I2C_EEPROM_H_
#define INC_I2C_EEPROM_H_

#include "main.h"

/* BASED ON EEPROM 24C64 */

#define EEPROM_ADDR         0xA0
#define EEPROM_IIC_TIMEOUT  10
#define EEPROM_MAX_PAGE_DIM 31

typedef enum
{
	EEPROM_NO_ERROR = 0,
	EEPROM_GENERAL_ERROR
}Eeprom_error_t;

typedef enum{
	EEPROM_BYTE_WRITE = 0,
	EEPROM_PAGE_WRITE
}Eeprom_write_mode_t;

typedef enum{
	EEPROM_RANDOM_READ = 0,
	EEPROM_SEQUENTIAL_READ
}Eeprom_read_mode_t;

void EepromIicInit(void);
Eeprom_error_t EepromWrite(const uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_write_mode_t mode);
Eeprom_error_t EepromRead(uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_read_mode_t mode);

#endif
