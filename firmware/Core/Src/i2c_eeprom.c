#include "i2c_eeprom.h"

static Eeprom_error_t I2c2CheckStatus(uint32_t flag);

I2C_HandleTypeDef hi2c2;
uint32_t i2c2_time = 0;

/****************************************************************************
Function:			static Eeprom_error_t I2c2CheckStatus(uint32_t flag)
Input:				none
Output:				none
PreCondition:		none
Overview:			Initialize I2C2 peripheral
****************************************************************************/
void EepromIicInit(void)
{
	RCC->APB1ENR              |= RCC_APB1ENR_I2C2EN;
	hi2c2.Instance             = I2C2;
	hi2c2.Init.ClockSpeed      = 100000;
	hi2c2.Init.DutyCycle       = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1     = 0;
	hi2c2.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2     = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c2);
}

/****************************************************************************
Function:     Eeprom_error_t EepromWrite(const uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_write_mode_t mode)
Input:	      const uint8_t *data            -> pointer to buffer
              uint8_t len                    -> len of buffer to be written into memory
              uint16_t eeprom_memory_address -> address of the 24c64 eeprom
              Eeprom_write_mode_t mode       -> mode of write procedure
Output:		  EEPROM_GENERAL_ERROR if error, EEPROM_NO_ERROR if success
PreCondition: call to EepromIicInit() and EepromPinInit()
Overview:	  none
****************************************************************************/
Eeprom_error_t EepromWrite(const uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_write_mode_t mode)
{
	uint8_t i;

	/* 24C64 supports only 31 sequential bytes in page write mode*/
	if(len > EEPROM_MAX_PAGE_DIM)
		return EEPROM_GENERAL_ERROR;

	if ( (mode != EEPROM_BYTE_WRITE) && (mode != EEPROM_PAGE_WRITE) )
		return EEPROM_GENERAL_ERROR;

	i2c2_time = HAL_GetTick();

	/* Check for peripheral activation */
	if((hi2c2.Instance->CR1 & I2C_CR1_PE) == 0 )
		return EEPROM_GENERAL_ERROR;

	hi2c2.Instance->CR1 |= I2C_CR1_START;

	/* Generation of start condition */
	if (I2c2CheckStatus(I2C_SR1_SB))
		return EEPROM_GENERAL_ERROR;

	/* Selecting type of transmission -> write */
	hi2c2.Instance->DR =  EEPROM_ADDR & 0xFE;

	/* Send address and wait for complete address transfer and ack */
	if (I2c2CheckStatus(I2C_SR1_ADDR))
		return EEPROM_GENERAL_ERROR;
	hi2c2.Instance->SR1;
	hi2c2.Instance->SR2;

	/* Check for transmitter register empty */
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	/* Send address byte MSB */
	hi2c2.Instance->DR  = (uint8_t) (eeprom_memory_address >> 8);
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	/* Send address byte LSB */
	hi2c2.Instance->DR  = (uint8_t) eeprom_memory_address;
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	if(mode == EEPROM_BYTE_WRITE)
	{
	/* Send single byte of data */
	hi2c2.Instance->DR  = *data;
	/* check for transmitter register empty */
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;
	}
	else
	{
		for(i=0; i<len; i++)
		{
			/* Send single byte of data and increment the pointer */
			hi2c2.Instance->DR  = *data;
			data++;
			/* check for transmitter register empty */
			if (I2c2CheckStatus(I2C_SR1_TXE))
				return EEPROM_GENERAL_ERROR;
		}
	}
	/* Generate stop condition */
	hi2c2.Instance->CR1 |= I2C_CR1_STOP;

	return EEPROM_NO_ERROR;
}

/****************************************************************************
Function:     Eeprom_error_t EepromRead(uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_read_mode_t mode)
Input:	      uint8_t *data                  -> pointer to buffer
	          uint8_t len                    -> len of bytes to be read
	          uint16_t eeprom_memory_address -> address of the 24c64 eeprom
	          Eeprom_write_mode_t mode       -> mode of read procedure
Output:		  EEPROM_GENERAL_ERROR if error, EEPROM_NO_ERROR if success
PreCondition: call to EepromIicInit() and EepromPinInit()
Overview:	  none
****************************************************************************/
Eeprom_error_t EepromRead(uint8_t *data, uint8_t len, uint16_t eeprom_memory_address, Eeprom_read_mode_t mode)
{
	uint8_t i;

	if ( (mode != EEPROM_RANDOM_READ) && (mode != EEPROM_SEQUENTIAL_READ) )
		return EEPROM_GENERAL_ERROR;

	i2c2_time = HAL_GetTick();

	/* Check for peripheral activation */
	if( (hi2c2.Instance->CR1 & I2C_CR1_PE) == 0 )
		return EEPROM_GENERAL_ERROR;

	/* Generation of start condition */
	hi2c2.Instance->CR1 |= I2C_CR1_START;

	/* checking generation of start condition */
	if (I2c2CheckStatus(I2C_SR1_SB))
		return EEPROM_GENERAL_ERROR;

	/* Selecting type of transmission(write)*/
	hi2c2.Instance->DR =  EEPROM_ADDR & 0xFE;

	/* Send address and wait for complete address transfer and ack */
	if (I2c2CheckStatus(I2C_SR1_ADDR))
		return EEPROM_GENERAL_ERROR;
	hi2c2.Instance->SR1;
	hi2c2.Instance->SR2;

	/* Check for transmitter register empty */
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	/* Send MSB address */
	hi2c2.Instance->DR  = (uint8_t) (eeprom_memory_address >> 8);
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	/* Send LSB address */
	hi2c2.Instance->DR  = (uint8_t) eeprom_memory_address;
	if (I2c2CheckStatus(I2C_SR1_TXE))
		return EEPROM_GENERAL_ERROR;

	/* Generate start condition */
	hi2c2.Instance->CR1 |= I2C_CR1_START;

	/* Checking generation of start condition */
	if (I2c2CheckStatus(I2C_SR1_SB))
		return EEPROM_GENERAL_ERROR;

	/* Repeat address(read) and clear SB bit */
	hi2c2.Instance->SR1;
	hi2c2.Instance->DR = EEPROM_ADDR | 0x01;
	if (I2c2CheckStatus(I2C_SR1_ADDR))
		return EEPROM_GENERAL_ERROR;
	hi2c2.Instance->SR1;
	hi2c2.Instance->SR2;

	if(mode == EEPROM_RANDOM_READ)
	{
		/* RANDOM READ MODE */
		hi2c2.Instance->CR1 |= I2C_CR1_STOP;
		hi2c2.Instance->CR1 &= ~I2C_CR1_ACK;
		if (I2c2CheckStatus(I2C_SR1_RXNE))
			return EEPROM_GENERAL_ERROR;
		*data = hi2c2.Instance->DR;
	}
	else
	{
		/* SEQUENTIAL READ MODE */
		for(i=0; i<(len - 1); i++)
		{
			hi2c2.Instance->CR1 |= I2C_CR1_ACK;
			if (I2c2CheckStatus(I2C_SR1_RXNE))
				return EEPROM_GENERAL_ERROR;
			*data = hi2c2.Instance->DR;
			 data++;
		}
		hi2c2.Instance->CR1 |= I2C_CR1_STOP;
		hi2c2.Instance->CR1 &= ~I2C_CR1_ACK;
		if (I2c2CheckStatus(I2C_SR1_RXNE))
			return EEPROM_GENERAL_ERROR;
		*data = hi2c2.Instance->DR;
	}
	return EEPROM_NO_ERROR;
}

/****************************************************************************
Function:			static Eeprom_error_t I2c2CheckStatus(uint32_t flag)
Input:				uint32_t flag (byte og the status register of I2C2)
Output:				none
PreCondition:		none
Overview:			none
****************************************************************************/
static Eeprom_error_t I2c2CheckStatus(uint32_t flag)
{
	Eeprom_error_t bus_err = EEPROM_NO_ERROR;
	while ((hi2c2.Instance->SR1 & flag) == 0)
	{
		if( (HAL_GetTick() - i2c2_time ) >= EEPROM_IIC_TIMEOUT)
		{
			bus_err = EEPROM_GENERAL_ERROR;
			break;
		}
	}
	return bus_err;
}
