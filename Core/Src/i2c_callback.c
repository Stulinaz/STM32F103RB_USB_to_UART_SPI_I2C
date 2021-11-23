#include"i2c_callback.h"
#include "gpio.h"

/* I2C trasmitter mode at page 757 */
/* I2C receiver   mode at page 761 */

static void I2cPollFlagSR1(i2c_error_t *const status, uint32_t flag);
static i2c_error_t I2cFailureDetect(void);

I2C_HandleTypeDef hi2c1;
uint32_t time_start;

void MX_I2C1_Init(void)
{
	RCC->APB1ENR              |= RCC_APB1ENR_I2C1EN;
	hi2c1.Instance             = I2C1;
	hi2c1.Init.ClockSpeed      = 80000;
	hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1     = 0;
	hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2     = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
	I2cPinConfig();
}

void I2CHardReset(void)
{
	HAL_I2C_DeInit(&hi2c1);
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
}

static void I2cPollFlagSR1(i2c_error_t *const status, uint32_t flag)
{
	i2c_error_t bus_err;
	while ((hi2c1.Instance->SR1 & flag) == 0)
	{
		bus_err = I2cFailureDetect();
		if (bus_err)
		{
			(*status) = bus_err;
			return;
		}
	}
}

static i2c_error_t I2cFailureDetect(void)
{
	/* timeout check - TIMEOUT bit is SR1 is set in 25ms*/
	if ((HAL_GetTick() - time_start) >= IIC_TIMEOUT)
		return I2C_TIMEOUT;

	if ((hi2c1.Instance->SR1 & I2C_SR1_OVR) != 0)
		return I2C_OVERRUN_UNDERRUN;

	/* Set when no acknowledge is returned*/
	if ((hi2c1.Instance->SR1 & I2C_SR1_AF) != 0)
		return I2C_ACKNOWLEDGE_FAILURE;

	/*Arbitration Lost - another master on the bus*/
	if ((hi2c1.Instance->SR1 & I2C_SR1_ARLO) != 0)
		return I2C_ARBITRATION_LOST;

	if ((hi2c1.Instance->SR1 & I2C_SR1_BERR) != 0)
		return I2C_BUS_ERROR;
	return NO_ERROR;
}

void Call_I2cCheckActivation (i2c_error_t *const status_transfer)
{
	time_start = HAL_GetTick();
	/* check for peripheral activation */
	if((hi2c1.Instance->CR1 & I2C_CR1_PE) == 0 )
		(*status_transfer) = I2C_NOT_ACTIVATED;
}

void Call_I2cGenerateStart(i2c_error_t *const status_transfer)
{
	uint8_t reg;
	/* generation of start condition */
	hi2c1.Instance->CR1 |= I2C_CR1_START;

	/* checking generation of start condition */
	I2cPollFlagSR1(status_transfer, I2C_SR1_SB);

	/* clear SB bit*/
	if(status_transfer == NO_ERROR)
		reg = hi2c1.Instance->SR1;
}

void Call_I2cAddressSend(i2c_error_t *const status_transfer, uint8_t byte)
{
	uint8_t sr1;
	uint8_t sr2;
	hi2c1.Instance->DR = byte;
	/*send address and wait for complete address transfer and ack*/
	I2cPollFlagSR1(status_transfer, I2C_SR1_ADDR);
	if ( (*status_transfer) == NO_ERROR)
	{
		sr1 = hi2c1.Instance->SR1;
		sr2 = hi2c1.Instance->SR2;
	}
}

void Call_I2cSendData(i2c_error_t *const status_transfer, uint8_t data)
{
		hi2c1.Instance->DR  = data;
		/*check for transmitter register empty */
		I2cPollFlagSR1(status_transfer, I2C_SR1_TXE);
}

void Call_I2cReadData(i2c_error_t *const status_transfer, uint8_t *const data)
{
	I2cPollFlagSR1(status_transfer, I2C_SR1_RXNE);
	if( (*status_transfer) == NO_ERROR)
		(*data) = hi2c1.Instance->DR;
}

void Call_I2cSendStopCondition(i2c_error_t *const status_transfer)
{
	/* stop condition */
	hi2c1.Instance->CR1 |= I2C_CR1_STOP;
}

/*
i2c_error_t i2c_transfer_write (I2C_HandleTypeDef * const i2c, const uint8_t *buff, uint16_t len)
{
	uint8_t SR1 = 0;
	uint8_t SR2 = 0;
	uint8_t i;
	i2c_error_t stat = NO_ERROR;

	i2c_transfer.time = HAL_GetTick();

	 check for peripheral activation
	if((i2c->Instance->CR1 & I2C_CR1_PE) == 0 )
		stat = I2C_NOT_ACTIVATED;

	i2c->Instance->CR1 |= I2C_CR1_START;
	 generation of start condition
	I2cPollFlagSR1(&stat, I2C_SR1_SB);

	 selecting type of transmission read/write
	if(i2c_transfer.transmission_type == MASTER_TRANSMITTER)
		i2c->Instance->DR = i2c_transfer.slave_addr & 0xFE;
	else
		i2c->Instance->DR = i2c_transfer.slave_addr | 0x01;

	send address and wait for complete address transfer and ack
	I2cPollFlagSR1(&stat, I2C_SR1_ADDR);
	SR1 = i2c->Instance->SR1;
	SR2 = i2c->Instance->SR2;

	check for transmitter register empty
	I2cPollFlagSR1(&stat, I2C_SR1_TXE);

	send data and stop condition
	for(i = 0; i< len; i++)
	{
		i2c->Instance->DR  = *buff;
		buff++;
		I2cPollFlagSR1(&stat, I2C_SR1_TXE);
	}
	i2c->Instance->CR1 |= I2C_CR1_STOP;
	return stat;
}

i2c_error_t i2c_transfer_read (I2C_HandleTypeDef * const i2c)
{
	uint8_t SR1 = 0;
	uint8_t SR2 = 0;
	uint8_t i;
	i2c_error_t stat = NO_ERROR;

	i2c_transfer.time = HAL_GetTick();

	 check for peripheral activation
	if((i2c->Instance->CR1 & I2C_CR1_PE) == 0 )
		stat = I2C_NOT_ACTIVATED;

	 generation of start condition
	i2c->Instance->CR1 |= I2C_CR1_START;
	 checking generation of start condition
	I2cPollFlagSR1(&stat, I2C_SR1_SB);

	 selecting type of transmission read/write
	 clear SB bit
	SR1 = i2c->Instance->SR1;
	if(i2c_transfer.transmission_type == MASTER_TRANSMITTER)
		i2c->Instance->DR = i2c_transfer.slave_addr & 0xFE;
	else
		i2c->Instance->DR = i2c_transfer.slave_addr | 0x01;

	send address and wait for complete address transfer and ack
	I2cPollFlagSR1(&stat, I2C_SR1_ADDR);

	SR1 = i2c->Instance->SR1;
	SR2 = i2c->Instance->SR2;

	check for transmitter register empty
	I2cPollFlagSR1(&stat, I2C_SR1_TXE);

	send address high byte
	i2c->Instance->DR  = 0x00;
	I2cPollFlagSR1(&stat, I2C_SR1_TXE);

	 send address low byte
	i2c->Instance->DR  = 0x00;
	I2cPollFlagSR1(&stat, I2C_SR1_TXE);

	generate start condition
	i2c->Instance->CR1 |= I2C_CR1_START;
	 checking generation of start condition
	I2cPollFlagSR1(&stat, I2C_SR1_SB);

	 repeat address and clear SB bit
	SR1 = i2c->Instance->SR1;
	i2c->Instance->DR = i2c_transfer.slave_addr | 0x01;

	I2cPollFlagSR1(&stat, I2C_SR1_ADDR);
	SR1 = i2c->Instance->SR1;
	SR2 = i2c->Instance->SR2;

	I2cPollFlagSR1(&stat, I2C_SR1_RXNE);
	i = i2c->Instance->DR;

	return stat;
}
*/
