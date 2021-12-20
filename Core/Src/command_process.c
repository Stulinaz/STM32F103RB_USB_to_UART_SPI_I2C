#include "sys_definitions.h"
#include "command_process.h"
#include "usbd_cdc_if.h"
#include <string.h>

const char ascii_char[]     = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const uint8_t ascii_value[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

static uint8_t CharToHex(char msb, char lsb, uint8_t *const value);

/****************************************************************************
Function:			Decode
Input:				cmd_len = lenght of commadn from Pc, *param = return value of some commands
Output:				if command match return the command enum, otherwise UNKNOWN_COMMAND
PreCondition:		USBInit()
Overview:			Command handler
****************************************************************************/
command_t Decode(uint16_t cmd_len, uint8_t *const param)
{
	uint8_t buff[TEMP_BUFF_DIM];
	uint8_t i;

	if(cmd_len < MIN_CMD_LEN)
		return UNKNOWN_COMMAND;

	for(i=0; i<TEMP_BUFF_DIM ;i++)
		buff[i]=0;

	/* store user input in temporary buffer */
	for(i=0; i < cmd_len; i++)
		getbyte(USBVCP, buff+i);

	/* parse space char */
	if( !ParseCommand(cmd_len, buff) )
		return UNKNOWN_COMMAND;

	/* get new len after parsing */
	cmd_len = strlen(buff);

	/* general */
	if(memcmp(buff, help, strlen(help)) == 0)
		return HELP;

	if(memcmp(buff, serial_cmd_select, strlen(serial_cmd_select)) == 0)
		return USER_SERIAL_INTERFACE_SELECTED;

	if(memcmp(buff, i2c_cmd_select, strlen(i2c_cmd_select)) == 0)
		return USER_I2C_INTERFACE_SELECTED;

	if(memcmp(buff, spi_cmd_select, strlen(spi_cmd_select)) == 0)
		return USER_SPI_INTERFACE_SELECTED;

	if(memcmp(buff, ch1_on, strlen(ch1_on)) == 0)
		return USER_CH1_ACTIVATED;

	if(memcmp(buff, ch1_off, strlen(ch1_off)) == 0)
		return USER_CH1_DEACTIVATE;

	if(memcmp(buff, ch2_on, strlen(ch2_on)) == 0)
		return USER_CH2_ACTIVATED;

	if(memcmp(buff, ch2_off, strlen(ch2_off)) == 0)
		return USER_CH2_DEACTIVATE;

	if(memcmp(buff, test_led, strlen(test_led)) == 0)
		return USER_TEST_ONBOARD_LED;

	if(memcmp(buff, close_interface, strlen(close_interface)) == 0)
		return USER_CLOSE_INTERFACE;

	/* serial */
    if(memcmp(buff, serial_bausel, strlen(serial_bausel)) == 0)
	{
		if( !CharToHex('0', *(buff+(cmd_len-1)), param) )
			return USER_SER_SET_BAUDRATE;
	}

    /*spi*/
    if(memcmp(buff, spi_clkpollow, strlen(spi_clkpollow)) == 0)
		return USER_SPI_CLK_POLARITY_LOW;

    if(memcmp(buff, spi_clkpolhigh, strlen(spi_clkpolhigh)) == 0)
		return USER_SPI_CLK_POLARITY_HIGH;

    if(memcmp(buff, spi_speed, strlen(spi_speed)) == 0)
    {
    	if( !CharToHex('0', *(buff+(cmd_len-1)), param) )
    	return USER_SPI_SPEED_SELECTION;
    }

    if(memcmp(buff, spi_nssactivelow, strlen(spi_nssactivelow)) == 0)
		return USER_SPI_NSS_ACTIVE_LOW;

    if(memcmp(buff, spi_nssactivehigh, strlen(spi_nssactivehigh)) == 0)
		return USER_SPI_NSS_ACTIVE_HIGH;

	/* iic */
	if(memcmp(buff, i2c_address, strlen(i2c_address)) == 0)
	{
		if( !CharToHex(*(buff+(cmd_len-2)), *(buff+(cmd_len-1)), param) )
			return USER_I2C_SET_SLAVE_ADDRESS;
	}

	if(memcmp(buff, i2c_access_write, strlen(i2c_access_write)) == 0)
	{
		if( !CharToHex(*(buff+(cmd_len-2)), *(buff+(cmd_len-1)), param) )
			return USER_I2C_WRITE_DATA;
	}

	if(memcmp(buff, i2c_transmitter, strlen(i2c_transmitter)) == 0)
		return USER_IC2_ACCESS_MODE_TRANSMITTER;

	if(memcmp(buff, i2c_receiver, strlen(i2c_receiver)) == 0)
		return USER_IC2_ACCESS_MODE_RECEIVER;

	if(memcmp(buff, i2c_access_read, strlen(i2c_access_read)) == 0)
		return USER_I2C_READ_DATA;

	if(memcmp(buff, i2c_repeat_start, strlen(i2c_repeat_start)) == 0)
		return USER_I2C_REPEAT_START;

	if(memcmp(buff, i2c_queue_req, strlen(i2c_queue_req)) == 0)
		return USER_I2C_REQUEST_QUEUE_DATA;

	if(memcmp(buff, i2c_queue_delete, strlen(i2c_queue_delete)) == 0)
		return USER_I2C_REQUEST_QUEUE_DELETE;

	if(memcmp(buff, transfer_start, strlen(transfer_start)) == 0)
		return USER_TRANSFER_REQUEST;

#ifdef MIPOT_TRX_PROGRAM_ENABLED
	if(memcmp(buff, mip_impulse, strlen(mip_impulse)) == 0)
		return USER_MIP_TRX_GENERATE_IMPULSE;

	if(memcmp(buff, mip_program, strlen(mip_program)) == 0)
		return USER_MIP_TRX_PROGRAM_MODULE;
#endif

	return UNKNOWN_COMMAND;
}

/****************************************************************************
Function:			CharToHex
Input:				Two byte (User - Friendly data)
Example:            (msb ='0' lsb ='A') -> Conversion ok -> value = 10 -> return value = 0
                    (msb ='0' lsb ='A') -> Conversion ok -> value = 10 -> return value = 0
                    (msb ='1' lsb ='G') -> Conversion ko -> value = 0  -> return value != 0
Output:				0 if conversion is ok, != 0 otherwise
PreCondition:		None
Overview:			Convert one byte of data into hexadecimal format (2byte)
****************************************************************************/
static uint8_t CharToHex(char msb, char lsb, uint8_t *const value)
{
	uint8_t i;
	uint8_t stat = 2;
	(*value) = 0;
	for(i =0; i<16; i++)
	{
		if(msb == ascii_char[i])
		{
			stat--;
			(*value) += ascii_value[i] * 16;
		}
		if(lsb == ascii_char[i])
		{
			stat--;
			(*value) += ascii_value[i];
		}
	}
	if (stat)
		(*value) = 0;
	return stat;
}

/****************************************************************************
Function:			DecToChar
Input:				byte
Output:				User friendly data in hex format (FFh)
Example:            Input: 255 (one byte) -> Output : FFh (two byte)
					Input: 1  (one byte)  -> Output : 01h (two byte)
PreCondition:		None
Overview:			Convert one byte of data into hexadecimal format (2byte)
****************************************************************************/
uint16_t DecToChar(uint8_t value)
{
	uint8_t i;
	uint8_t msb_nibble = value / 16;
	uint8_t lsb_nibble = value % 16;
	uint8_t conversion_lsb;
	uint16_t conversion_msb;

	for(i =0; i<16; i++)
	{
		if(msb_nibble == ascii_value[i])
			conversion_msb = ((uint16_t)ascii_char[i]) & 0x00FF;
		if(lsb_nibble == ascii_value[i])
			conversion_lsb = ((uint16_t)ascii_char[i]) & 0x00FF;
	}
	conversion_msb = conversion_msb << 8;
	return (conversion_msb | (uint16_t)conversion_lsb);
}

/****************************************************************************
Function:			ParseCommand
Input:				usb buffer
Output:				buffer parsed with space char
PreCondition:		None
Overview:
****************************************************************************/
uint8_t ParseCommand(uint16_t len, uint8_t *buff)
{
	uint16_t i,t;
	uint16_t stat = 0;
	/* checking the presence of all empty char */
	for(i=0; i<len; i++)
	{
		if(buff[i] != CHAR_SPACE)
			stat++;
	}
	if(!stat)
		return FAIL;

	for(t=0;t<len;t++)
	{
		for(i=0; i<len; i++)
		{
			if(buff[i] == CHAR_SPACE)
			{
				if( (buff[i+1] != '\0') && (buff[i+1] != CHAR_SPACE) )
				{
					/* swap */
					buff[i] = buff[i+1];
					buff[i+1] = CHAR_SPACE;
				}
			}
		}
	}
	for(i=0; i<len; i++)
	{
		/* Delete space */
		if (buff[i] == CHAR_SPACE)
			buff[i] = '\0';
	}
	return PASS;
}
