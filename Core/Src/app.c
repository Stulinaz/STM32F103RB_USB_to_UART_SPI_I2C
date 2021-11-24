#include "app.h"
#include "led_manager.h"
#include "buffers_manager.h"
#include "i2c.h"
#include "serial.h"
#include "spi.h"

command_t communication_mode = NO_COMMAND;

static void AppToTx(comm_inerface_t mode);
static void I2cScanQueue(void);
static void PrintHelp(void);

/****************************************************************************
Function:			Application
Input:				(command_t * user_cmd) last command correctly decoded
					(uint8_t * const user_data) parameter decoded
Overview:			Application Data Transfer
****************************************************************************/
void Application (command_t * user_cmd, uint8_t * const user_data)
{
	switch(*user_cmd)
	{
		case NO_COMMAND:
			break;

		case UNKNOWN_COMMAND:
		if(communication_mode == USER_SERIAL_INTERFACE_SELECTED)
			AppToTx(SER_INTERFACE);
		else if(communication_mode == USER_SPI_INTERFACE_SELECTED)
			AppToTx(SPI_INTERFACE);
		else
			UsbPrintString("unknown command", PRINT_ONLY);
		break;

		case USER_SERIAL_INTERFACE_SELECTED:
		UartStart();
		LedInterfaceSel(USER_SERIAL_INTERFACE_SELECTED);
		communication_mode = USER_SERIAL_INTERFACE_SELECTED;
		UsbPrintString("Serial interface selected", PRINT_ONLY);
		break;

		case USER_I2C_INTERFACE_SELECTED:
		LedInterfaceSel(USER_I2C_INTERFACE_SELECTED);
		communication_mode = USER_I2C_INTERFACE_SELECTED;
		UsbPrintString("I2C interface selected", PRINT_ONLY);
		break;

		case USER_SPI_INTERFACE_SELECTED:
		SPIStart();
		LedInterfaceSel(USER_SPI_INTERFACE_SELECTED);
		communication_mode = USER_SPI_INTERFACE_SELECTED;
		UsbPrintString("SPI interface selected", PRINT_ONLY);
		break;

		/* Serial */
		case USER_SER_SET_BAUDRATE:
		if(UartUSerConf(*user_cmd, *user_data) == UART_CONFIGURATION_OK)
			UsbPrintString("Serial baudrate set", PRINT_ONLY);
		else
			UsbPrintString(wrong_param, PRINT_ONLY);
		break;

		/* SPI */
		case USER_SPI_CLK_POLARITY_LOW:
		case USER_SPI_CLK_POLARITY_HIGH:
		case USER_SPI_SPEED_SELECTION:
		if(SpiUserConf(*user_cmd, *user_data) == SPI_CONFIGURATION_OK)
			UsbPrintString("Spi parameter set", PRINT_ONLY);
		else
			UsbPrintString(wrong_param, PRINT_ONLY);
		break;


		/* I2C */
		case USER_I2C_SET_SLAVE_ADDRESS:
		I2cSetSlaveAddress(*user_data);
		UsbPrintString("Slave address set", PRINT_ONLY);
		break;

		case USER_IC2_ACCESS_MODE_TRANSMITTER:
		if (I2cEnqeue(MASTER_TRANSMITTER , ADDRESS_TRANSFER))
			UsbPrintString("Sending address in write mode", PRINT_ONLY);
		else
			UsbPrintString(queue_full, TRUE);
		break;

		case USER_IC2_ACCESS_MODE_RECEIVER:
		if (I2cEnqeue(MASTER_RECEIVER , ADDRESS_TRANSFER))
			UsbPrintString("Sending address in read mode", PRINT_ONLY);
		else
			UsbPrintString(queue_full, PRINT_ONLY);
		break;

		case USER_I2C_WRITE_DATA:
		if (I2cEnqeue(*user_data, BYTE_WRITE) )
			UsbPrintString("Byte transfer enqueued", PRINT_ONLY);
		else
			UsbPrintString(queue_full, PRINT_ONLY);
		break;

		case USER_I2C_READ_DATA:
		if (I2cEnqeue( NUL, BYTE_READ) )
			UsbPrintString("Byte read enqueued", PRINT_ONLY);
		else
			UsbPrintString(queue_full, PRINT_ONLY);
		break;

		case USER_I2C_REPEAT_START:
		if(I2cEnqeue( NUL, REPEAT_START))
			UsbPrintString("Repeat start condition enqueued", PRINT_ONLY);
		else
			UsbPrintString(queue_full, PRINT_ONLY);
		break;

		case USER_I2C_REQUEST_QUEUE_DATA:
		I2cScanQueue();
		break;

		case USER_I2C_REQUEST_QUEUE_DELETE:
		I2cQueueRelease();
		UsbPrintString("I2C queue free", PRINT_ONLY);
		break;

		case USER_TRANSFER_REQUEST:
			//TODO
		if(communication_mode != USER_I2C_INTERFACE_SELECTED)
			AppToTx(I2C_INTERFACE);
		else
			UsbPrintString("No Interface selected", PRINT_ONLY);
		break;

		/* Power Driver */
		case USER_CH1_ACTIVATED:
		HAL_GPIO_WritePin(DRIVER_CH1_GPIO_Port, DRIVER_CH1_Pin, GPIO_PIN_SET);
		ErrorCodeSet(1);
		UsbPrintString("Ch1 ON", PRINT_ONLY);
		break;

		case USER_CH1_DEACTIVATE:
		HAL_GPIO_WritePin(DRIVER_CH1_GPIO_Port, DRIVER_CH1_Pin, GPIO_PIN_RESET);
		ErrorCodeSet(0);
		UsbPrintString("Ch1 OFF", PRINT_ONLY);
		break;

		case USER_CH2_ACTIVATED:
		HAL_GPIO_WritePin(DRIVER_CH2_GPIO_Port, DRIVER_CH2_Pin, GPIO_PIN_SET);
		ErrorCodeSet(2);
		UsbPrintString("Ch2 ON", PRINT_ONLY);
		break;

		case USER_CH2_DEACTIVATE:
		HAL_GPIO_WritePin(DRIVER_CH2_GPIO_Port, DRIVER_CH2_Pin, GPIO_PIN_RESET);
		ErrorCodeSet(0);
		UsbPrintString("Ch1 OFF", PRINT_ONLY);
		break;

		case HELP:
		PrintHelp();
		break;

		default:
			break;
	}
}

/****************************************************************************
Function:			I2cScanQueue
Input:				none
Output:				none
PreCondition:		none
Overview:			Scan the I2c commands queue and output to the user
****************************************************************************/
static void I2cScanQueue(void)
{
	i2c_queue_t cmd;
	uint8_t byte        = 0;
	uint16_t conversion = 0;
	uint8_t i           = 0;

	while(I2cGetqueue( &byte, &cmd, i++))
	{
		switch(cmd)
		{
			case REPEAT_START:
			UsbPrintString("Start / Repeat Start", APPEND_CR);
			break;

			case ADDRESS_TRANSFER:
			if ( (byte & MASTER_RECEIVER) != 0 )
				UsbPrintString("Read mode ", PRINT_ONLY);
			else
			UsbPrintString("Write mode ", PRINT_ONLY);
			conversion = DecToChar(byte);
			UsbPrintString("Addr: ", PRINT_ONLY);
			putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
			putbyte(USB_INTERFACE, (uint8_t) conversion );
			UsbPrintString("h ", APPEND_CR);
			break;

			case BYTE_WRITE:
			conversion = DecToChar(byte);
			UsbPrintString("Byte write: ", PRINT_ONLY);
			putbyte(USB_INTERFACE, (uint8_t)(conversion >> 8));
			putbyte(USB_INTERFACE, (uint8_t) conversion );
			UsbPrintString("h ", APPEND_CR);
			break;

			case BYTE_READ:
			UsbPrintString("Byte Read", APPEND_CR);
			break;
			default:
			break;
		}
	}
}

/****************************************************************************
Function:			AppToTx
Input:				command_t mode (selected working interface UART - SPI - I2C)
Output:				none
PreCondition:		none
Overview:			Start data transfer on selected working interface
****************************************************************************/
static void AppToTx(comm_inerface_t mode)
{
	i2c_queue_t cmd;
	uint16_t data = 0;
	uint8_t byte  = 0;
	uint8_t i     = 0;
	switch (mode)
	{
		case SER_INTERFACE:
		data = SetBuffer(SER_INTERFACE);
		if(data)
		{
			/* Enable USART TX interrupt*/
			SerStartTransmit();
		}
		break;

		case I2C_INTERFACE:
		data = (uint16_t) I2cTransfer();
		if(data)
		{
			/* Error on I2C bus*/
			#ifndef I2C_ERROR_VERBOSE
			data = DecToChar(data);
			UsbPrintString("Transfer Failure - error code:", PRINT_ONLY);
			putbyte(USB_INTERFACE, (uint8_t)(data  >> 8));
			putbyte(USB_INTERFACE, (uint8_t) data  );
			UsbPrintString("h ", APPEND_CR);
			#else
			//TODO verbose level i2c error
			#endif
		}
		else
		{
			/* No Error on I2C bus*/
			/* Output data if user request i2c read*/
			while(I2cGetqueue( &byte, &cmd, i++))
			{
				if(cmd == BYTE_READ)
				{
					data = DecToChar(byte);
					UsbPrintString("Byte read: ", APPEND_CR);
					putbyte(USB_INTERFACE, (uint8_t)(data >> 8));
					putbyte(USB_INTERFACE, (uint8_t) data );
				}
			}
			UsbPrintString("Transfer Success", APPEND_CR);
		}
		break;

		case SPI_INTERFACE:
		data = SetBuffer(SPI_INTERFACE);
		if(data)
		{
			/* Start transfer in polling mode*/
			SpiSend();
		}
		break;
		default:
			break;
	}
}

void TransferToPc(void)
{
	switch (communication_mode)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
			ToUsb(SER_INTERFACE);
		break;
		case USER_SPI_INTERFACE_SELECTED:
			ToUsb(SPI_INTERFACE);
		break;
		case USER_I2C_INTERFACE_SELECTED:
		default:
			break;
	}

}

/****************************************************************************
Function:			PrintHelp
Input:				none
Output:				none
PreCondition:		Must not exceed APP_TX_DATA_SIZE buffer size
Overview:			Print a help string to the User on Virtual Com Port
****************************************************************************/
static void PrintHelp(void)
{
	/*general command + options*/
	UsbPrintString(ch1_on, PRINT_ONLY);
	UsbPrintString( " :CH1 ON", APPEND_CR);

	UsbPrintString(ch1_off, PRINT_ONLY);
	UsbPrintString( " :CH1 OFF", APPEND_CR);

	UsbPrintString(ch2_on, PRINT_ONLY);
	UsbPrintString( " :CH2 ON", APPEND_CR);

	UsbPrintString(ch2_off, PRINT_ONLY);
	UsbPrintString( " :CH2 OFF", APPEND_CR);

	UsbPrintString(serial_cmd_select, PRINT_ONLY);
	UsbPrintString( " :serial interface ON", APPEND_CR);

	UsbPrintString(spi_cmd_select, PRINT_ONLY);
	UsbPrintString( " :spi interface ON", APPEND_CR);

	UsbPrintString(i2c_cmd_select, PRINT_ONLY);
	UsbPrintString( " :i2c interface ON", APPEND_CRCR);

	/*serial command + options*/
	UsbPrintString(serial_bausel, PRINT_ONLY);
	UsbPrintString("X :Selection of baudrate", APPEND_CRCR);

	/*spi command + options*/
	UsbPrintString(spi_clkpollow, PRINT_ONLY);
	UsbPrintString("CLK to 0 when idle", APPEND_CR);

	UsbPrintString(spi_clkpolhigh, PRINT_ONLY);
	UsbPrintString("CLK to 1 when idle", APPEND_CR);

	UsbPrintString(spi_speed, PRINT_ONLY);
	UsbPrintString("X :clk speed", APPEND_CR);

	UsbPrintString(spi_nssactivehigh, PRINT_ONLY);
	UsbPrintString("NSS active low", APPEND_CR);

	UsbPrintString(spi_nssactivehigh, PRINT_ONLY);
	UsbPrintString("NSS active high", APPEND_CRCR);

	/*i2c command + options*/
	UsbPrintString(i2c_address, PRINT_ONLY);
	UsbPrintString("XX :select slave address", APPEND_CR);

	UsbPrintString(i2c_receiver, PRINT_ONLY);
	UsbPrintString(" :address read", APPEND_CR);

	UsbPrintString(i2c_transmitter, PRINT_ONLY);
	UsbPrintString(" :address write", APPEND_CR);

	UsbPrintString(i2c_access_write, PRINT_ONLY);
	UsbPrintString("XX :byte write", APPEND_CR);

	UsbPrintString(i2c_access_read, PRINT_ONLY);
	UsbPrintString(" :byte read", APPEND_CR);

	UsbPrintString(i2c_queue_req, PRINT_ONLY);
	UsbPrintString(" :print queue", APPEND_CR);

	UsbPrintString(i2c_queue_delete, PRINT_ONLY);
	UsbPrintString(" :delete queue", APPEND_CR);

	UsbPrintString(transfer_start, PRINT_ONLY);
	UsbPrintString(" :start i2c transfer", APPEND_CRCR);

	/* End string*/
	UsbPrintString("STM32F103RB", APPEND_CR);
}
