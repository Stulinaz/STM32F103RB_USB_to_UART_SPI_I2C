#include "i2c.h"
#include"i2c_callback.h"

typedef struct{
uint8_t            slave_addr;
uint8_t            transmission_type;
uint8_t     	   data[I2C_QUEUE_COMMAND_DIM];
i2c_queue_t        iic_queue[I2C_QUEUE_COMMAND_DIM];
i2c_error_t        iic_transfer_status;
}i2c_comm_param;

typedef struct
{
	void (*Start)         (void);
	void (*Check)         (i2c_error_t *const status_transfer);
	void (*GenerateStart) (i2c_error_t *const status_transfer);
	void (*AddressSend)   (i2c_error_t *const status_transfer, uint8_t byte);
	void (*SendData)      (i2c_error_t *const status_transfer, uint8_t data);
	void (*ReadData)      (i2c_error_t *const status_transfer, uint8_t *data);
	void (*Stop)          (i2c_error_t *const status_transfer);
}I2c_callback_t;

i2c_comm_param i2c_transfer  = {0xa0,
								MASTER_TRANSMITTER,
								{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							    {REPEAT_START,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								NO_ERROR};

I2c_callback_t iic_fcall = {MX_I2C1_Init,
						    Call_I2cCheckActivation,
							Call_I2cGenerateStart,
						    Call_I2cAddressSend,
						    Call_I2cSendData,
							Call_I2cReadData,
						    Call_I2cSendStopCondition};

_bool I2cEnqeue( uint8_t data, i2c_queue_t param)
{
	uint8_t i;
	uint8_t quque_pos = 0;
	_bool stat = FALSE;

	/* check queue */
	for(i=0; i<I2C_QUEUE_COMMAND_DIM; i++)
	{
		if(i2c_transfer.iic_queue[i] == NO_COMMAND_SELECTED)
		{
			quque_pos = i;
			stat = TRUE;
			break;
		}
	}
	if(!stat)
		return stat;

	/* check commands */
	switch(param)
	{
		case ADDRESS_TRANSFER:
		{
			i2c_transfer.iic_queue[quque_pos] =  ADDRESS_TRANSFER;
				if(data == MASTER_TRANSMITTER)
					i2c_transfer.data[quque_pos]  = i2c_transfer.slave_addr&0xFE;
				else
					i2c_transfer.data[quque_pos]  = i2c_transfer.slave_addr|0x01;
			break;
		}
		case BYTE_WRITE:
		case BYTE_READ:
		case REPEAT_START:
		case ACK_EXPECTED:
		case NACK_EXPECTED:
		{
			i2c_transfer.iic_queue[quque_pos] =  param;
			i2c_transfer.data[quque_pos]      =  data;
			stat = TRUE;
			break;
		}
		default:
			break;
	}
	return stat;
}

void I2cQueueRelease(void)
{
	uint8_t i;
	/* first start condition and address is not deleted*/
	for(i=I2C_OFFSET_TRANSF; i<I2C_QUEUE_COMMAND_DIM; i++)
	{
		i2c_transfer.iic_queue[i] =  NO_COMMAND_SELECTED;
		i2c_transfer.data[i]      =  NUL;
	}
}

_bool I2cGetqueue( uint8_t *const data, i2c_queue_t *const param, uint8_t queue_pos)
{
	_bool stat = FALSE;
	if(i2c_transfer.iic_queue[queue_pos] != NO_COMMAND_SELECTED)
	{
		(*param) = i2c_transfer.iic_queue[queue_pos];
		(*data)  = i2c_transfer.data[queue_pos];
		stat = TRUE;
	}
	return stat;
}

uint8_t I2cTransfer(void)
{
	uint8_t i = I2C_OFFSET_TRANSF;
	i2c_transfer.iic_transfer_status = NO_ERROR;

	/* Check activation of i2c peripheral*/
	iic_fcall.Check(&i2c_transfer.iic_transfer_status);
	if (i2c_transfer.iic_transfer_status != NO_ERROR)
	{
		iic_fcall.Start();
		i2c_transfer.iic_transfer_status = NO_ERROR;
	}

	/*Generation of start condition*/
	iic_fcall.GenerateStart(&i2c_transfer.iic_transfer_status);
	if (i2c_transfer.iic_transfer_status != NO_ERROR)
		return i2c_transfer.iic_transfer_status;

	/* keep calling commands untill error or empty command*/
	while ( (i2c_transfer.iic_queue[i] != NO_COMMAND_SELECTED) && (i2c_transfer.iic_transfer_status == NO_ERROR) )
	{
		switch(i2c_transfer.iic_queue[i])
		{
			case ADDRESS_TRANSFER:
			/* Send Slave address in master trasmitter or master receiver mode*/
			iic_fcall.AddressSend(&i2c_transfer.iic_transfer_status, i2c_transfer.data[i]);
			break;

			case BYTE_WRITE:
				iic_fcall.SendData(&i2c_transfer.iic_transfer_status, i2c_transfer.data[i]);
				if (i2c_transfer.iic_queue[i+1] == NO_COMMAND_SELECTED)
					iic_fcall.Stop(&i2c_transfer.iic_transfer_status);
			break;

			case BYTE_READ:
				/*stop condition must be set before the last byte read operation*/
				if (i2c_transfer.iic_queue[i+1] == NO_COMMAND_SELECTED)
						iic_fcall.Stop(&i2c_transfer.iic_transfer_status);
				iic_fcall.ReadData(&i2c_transfer.iic_transfer_status, &(i2c_transfer.data[i]));
			break;

			case REPEAT_START:
			iic_fcall.GenerateStart(&i2c_transfer.iic_transfer_status);

			case ACK_EXPECTED:
			break;

			case NACK_EXPECTED:
			break;

			default:
			break;
		}
		i++;
	}
	return i2c_transfer.iic_transfer_status;
}

void DBG_i2c(void)
{
	#ifdef READ
	I2cEnqeue( MASTER_RECEIVER, ADDRESS_TRANSFER);
	I2cEnqeue( 0x01, BYTE_WRITE);
	I2cEnqeue( 0x02, BYTE_WRITE);
	I2cEnqeue( NUL, REPEAT_START);
	I2cEnqeue( NUL, ADDRESS_TRANSFER);
	I2cEnqeue( NUL, BYTE_READ);
	#else
	I2cEnqeue( MASTER_TRANSMITTER, ADDRESS_TRANSFER);
	I2cEnqeue( 0x01, BYTE_WRITE);
	I2cEnqeue( 0x02, BYTE_WRITE);
	I2cEnqeue( 0x02, BYTE_WRITE);
	#endif
}

void I2cSetSlaveAddress(uint8_t addr)
{
	i2c_transfer.slave_addr = addr;
}
