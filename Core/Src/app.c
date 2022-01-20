#include "sys_definitions.h"
#include "app.h"
#include "led_manager.h"
#include "buffers_manager.h"
#include "i2c.h"
#include "i2c_callback.h"
#include "serial.h"
#include "spi.h"
#include "tim4_1us_tick.h"
#ifdef MIPOT_TRX_PROGRAM_ENABLED
#include "32001269_32001534.h"
#endif

comm_inerface_t communication_mode = IDLE;

static void AppToTx(comm_inerface_t mode);
static void I2cScanQueue(void);
static void PrintHelp(void);

/****************************************************************************
Function:			Application
Input:				(command_t * user_cmd) last command correctly decoded
					(uint8_t *  user_data) data decoded
					(uint8_t user_data_len) data len
Overview:			Application Data Transfer
****************************************************************************/
void Application (command_t * user_cmd, uint8_t *user_data, uint8_t user_data_len)
{
	switch(*user_cmd)
	{
		case NO_COMMAND:
			break;

		case USER_CLOSE_INTERFACE:
		if(communication_mode == UART)
		{
			UartStop();
			UsbPrintString("uart closed", PRINT_ONLY);
		}
		else if(communication_mode == SPI)
		{
			SPIStop();
			UsbPrintString("spi closed", PRINT_ONLY);
		}
		else if(communication_mode == I2C)
		{
			I2CHardReset();
			UsbPrintString("i2c closed", PRINT_ONLY);
		}
		LedInterfaceSel(IDLE);
		communication_mode = IDLE;
		break;

		case UNKNOWN_COMMAND:
		if(communication_mode == UART)
			AppToTx(UART);
		else if(communication_mode == SPI)
			AppToTx(SPI);
		else
			UsbPrintString("unknown command", PRINT_ONLY);
		break;

		case USER_SERIAL_INTERFACE_SELECTED:
		UartStart();
		LedInterfaceSel(UART);
		communication_mode = UART;
		UsbPrintString("Serial interface selected", PRINT_ONLY);
		break;

		case USER_I2C_INTERFACE_SELECTED:
		LedInterfaceSel(I2C);
		communication_mode = I2C;
		UsbPrintString("I2C interface selected", PRINT_ONLY);
		break;

		case USER_SPI_INTERFACE_SELECTED:
		SPIStart();
		LedInterfaceSel(SPI);
		communication_mode = SPI;
		UsbPrintString("SPI interface selected", PRINT_ONLY);
		break;

		/* Serial */
		case USER_SER_SET_BAUDRATE:
		if(communication_mode != UART)
		{
			if(UartUSerConf(*user_cmd, *user_data) == UART_CONFIGURATION_OK)
				UsbPrintString("Serial baudrate set", PRINT_ONLY);
			else
				UsbPrintString(wrong_param, PRINT_ONLY);
		}
		else
			UsbPrintString("Error-interface must be closed", PRINT_ONLY);
		break;

		/* SPI */
		case USER_SPI_CLK_POLARITY_LOW:
		case USER_SPI_CLK_POLARITY_HIGH:
		case USER_SPI_SPEED_SELECTION:
		if(communication_mode != SPI)
		{
			if(SpiUserConf(*user_cmd, *user_data) == SPI_CONFIGURATION_OK)
				UsbPrintString("Spi parameter set", PRINT_ONLY);
			else
				UsbPrintString(wrong_param, PRINT_ONLY);
		}
		else
			UsbPrintString("Error-interface must be closed", PRINT_ONLY);
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
		if(communication_mode != I2C)
			AppToTx(I2C);
		else
			UsbPrintString("i2c not selected", PRINT_ONLY);
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

		case USER_TEST_ONBOARD_LED:
		LedTest(10000);
		break;

#ifdef MIPOT_TRX_PROGRAM_ENABLED
		case USER_MIP_TRX_GENERATE_IMPULSE:
		if(communication_mode == UART)
		{
			HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_SET);
			TIM4_delay_us(80);
			HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_RESET);
			UsbPrintString("impulse generated on RTS pin", PRINT_ONLY);
		}
		else
			UsbPrintString("Err", PRINT_ONLY);
		break;

		case USER_MIP_TRX_PROGRAM_MODULE:
		if ( (communication_mode == UART) && (user_data_len) )
		{
			if(ProgramTRXModule(user_data, user_data_len) == MODULE_PROGRAMMING_SUCCESS)
				UsbPrintString("Success!!", PRINT_ONLY);
			else
				UsbPrintString("Module setup fail", PRINT_ONLY);
		}
		else
			UsbPrintString("Err", PRINT_ONLY);
		break;
#endif
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
			putbyte(USBVCP, (uint8_t)(conversion >> 8));
			putbyte(USBVCP, (uint8_t) conversion );
			UsbPrintString("h ", APPEND_CR);
			break;

			case BYTE_WRITE:
			conversion = DecToChar(byte);
			UsbPrintString("Byte write: ", PRINT_ONLY);
			putbyte(USBVCP, (uint8_t)(conversion >> 8));
			putbyte(USBVCP, (uint8_t) conversion );
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
		case UART:
		data = SetBuffer(UART);
		if(data)
		{
			/* Enable USART TX interrupt*/
			SerStartTransmit();
		}
		break;

		case I2C:
		data = (uint16_t) I2cTransfer();
		if(data)
		{
			/* Error on I2C bus*/
			data = DecToChar(data);
#ifndef I2C_ERROR_VERBOSE
			UsbPrintString("Transfer Failure - error code:", PRINT_ONLY);
			putbyte(USBVCP, (uint8_t)(data  >> 8));
			putbyte(USBVCP, (uint8_t) data  );
			UsbPrintString("h ", APPEND_CR);
#else
			switch(data)
			{
				case I2C_BUS_ERROR:
				UsbPrintString("Bus error", TRUE);
				break;

				case I2C_ARBITRATION_LOST:
				UsbPrintString("Arbitrarion lost error", TRUE);

				case I2C_TIMEOUT:
				UsbPrintString("Timeout", TRUE);
				break;

				case I2C_ADDRESS_NACK:
				UsbPrintString("Address NACK", TRUE);
				break;
				default:
					UsbPrintString("General Error", TRUE);
			}
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
					putbyte(USBVCP, (uint8_t)(data >> 8));
					putbyte(USBVCP, (uint8_t) data );
				}
			}
			UsbPrintString("Transfer Success", APPEND_CR);
		}
		break;

		case SPI:
		data = SetBuffer(SPI);
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
		case UART:
			ToUsb(UART);
		break;
		case SPI:
			ToUsb(SPI);
		break;
		case I2C:
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
	/* new line */
	putbyte(USBVCP, CR_);

	/*general command + options*/
	UsbPrintString(serial_cmd_select, PRINT_ONLY);
	UsbPrintString( " :serial interface ON", APPEND_CR);

	UsbPrintString(spi_cmd_select, PRINT_ONLY);
	UsbPrintString( " :spi interface ON", APPEND_CR);

	UsbPrintString(i2c_cmd_select, PRINT_ONLY);
	UsbPrintString( " :i2c interface ON", APPEND_CRCR);

	UsbPrintString(ch1_on, PRINT_ONLY);
	UsbPrintString( " :CH1 ON", APPEND_CR);

	UsbPrintString(ch1_off, PRINT_ONLY);
	UsbPrintString( " :CH1 OFF", APPEND_CR);

	UsbPrintString(ch2_on, PRINT_ONLY);
	UsbPrintString( " :CH2 ON", APPEND_CR);

	UsbPrintString(ch2_off, PRINT_ONLY);
	UsbPrintString( " :CH2 OFF", APPEND_CR);

	UsbPrintString(test_led, PRINT_ONLY);
	UsbPrintString( " :Turn on all onboard leds", APPEND_CR);

	UsbPrintString(close_interface, PRINT_ONLY);
	UsbPrintString( " :Turn off working interface", APPEND_CR);

	/*serial command + options*/
	UsbPrintString(serial_bausel, PRINT_ONLY);
	UsbPrintString("X :Selection of baudrate", APPEND_CRCR);

	/*spi command + options*/
	UsbPrintString(spi_clkpollow, PRINT_ONLY);
	UsbPrintString(" :CLK to 0 when idle", APPEND_CR);

	UsbPrintString(spi_clkpolhigh, PRINT_ONLY);
	UsbPrintString(" :CLK to 1 when idle", APPEND_CR);

	UsbPrintString(spi_speed, PRINT_ONLY);
	UsbPrintString("X :clk speed", APPEND_CR);

	UsbPrintString(spi_nssactivehigh, PRINT_ONLY);
	UsbPrintString(" :NSS active low", APPEND_CR);

	UsbPrintString(spi_nssactivehigh, PRINT_ONLY);
	UsbPrintString(" :NSS active high", APPEND_CRCR);

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

#ifdef MIPOT_TRX_PROGRAM_ENABLED
	UsbPrintString(mip_impulse, PRINT_ONLY);
	UsbPrintString(" :generate 100us impulse on RTS pin", APPEND_CR);

	UsbPrintString(mip_program, PRINT_ONLY);
	UsbPrintString(" :program module", APPEND_CRCR);
#endif
	/* End string*/
	UsbPrintString("STM32F103RBT6", APPEND_CR);
}
