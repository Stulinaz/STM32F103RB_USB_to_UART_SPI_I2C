#include "buffers_manager.h"
#include "sys_definitions.h"

uint8_t ser_tx_buff[SERIAL_TX_BUFF_DIM];
uint8_t ser_rx_buff[SERIAL_RX_BUFF_DIM];

uint8_t i2c_tx_buff[I2C_TX_BUFF_DIM];
uint8_t i2c_rx_buff[I2C_RX_BUFF_DIM];

uint8_t spi_tx_buff[SPI_TX_BUFF_DIM];
uint8_t spi_rx_buff[SPI_RX_BUFF_DIM];

static void InterfaceSelection(comm_interface_t type);

volatile comm_index_t ser_comm_type = { 0, 0, 0, 0, SERIAL_TX_BUFF_DIM, SERIAL_RX_BUFF_DIM };
volatile comm_index_t spi_comm_type = { 0, 0, 0, 0, SPI_TX_BUFF_DIM, SPI_RX_BUFF_DIM};
volatile comm_index_t i2c_comm_type = { 0, 0, 0, 0,  I2C_TX_BUFF_DIM, I2C_RX_BUFF_DIM};
volatile comm_index_t usb_comm_type = { 0, 0, 0, 0,  APP_TX_DATA_SIZE, APP_RX_DATA_SIZE};

volatile comm_interface_t working_interface = IDLE;
volatile comm_index_t *idx_strct_ptr;
volatile uint8_t      *buff_tx_ptr;
volatile uint8_t      *buff_rx_ptr;

/****************************************************************************
Function:			static void interface_selection (volatile comm_inerface_t type_t)
Input:				working interface
Output:				none
PreCondition:		none
Overview:			sed the pointers to the working interface for data handling
****************************************************************************/
static void InterfaceSelection(volatile comm_interface_t type)
{
	if(working_interface == type)
		return;
	switch(type)
	{
		case UART:
		idx_strct_ptr     = &ser_comm_type;
		buff_tx_ptr       = ser_tx_buff;
		buff_rx_ptr       = ser_rx_buff;
		working_interface = UART;
		break;

		case I2C:
		idx_strct_ptr     = &i2c_comm_type;
		buff_tx_ptr       = i2c_tx_buff;
		buff_rx_ptr       = i2c_rx_buff;
		working_interface = I2C;
		break;

		case SPI:
		idx_strct_ptr     = &spi_comm_type;
		buff_tx_ptr       = spi_tx_buff;
		buff_rx_ptr       = spi_rx_buff;
		working_interface = SPI;
		break;

		case USBVCP:
		idx_strct_ptr     = &usb_comm_type;
		buff_tx_ptr       = UserTxBufferFS;
		buff_rx_ptr       = UserRxBufferFS;
		working_interface = USBVCP;
		break;
		default:
			break;
	}
}

/****************************************************************************
Function:			uint8_t putbyte(comm_inerface_t comm_type, uint8_t data)
Input:				the interface where we want to send the data
Output:				FAIL if buffer is full, PASS is buffer has space to store data
PreCondition:		none
Overview:
****************************************************************************/
uint8_t putbyte(comm_interface_t comm_type, uint8_t data)
{
	uint8_t stat = FAIL;
	InterfaceSelection(comm_type);
	if (idx_strct_ptr->tx_buff_write_index < (idx_strct_ptr->tx_max_buff_dim))
	{
		/* Buffer is not full */
		*(buff_tx_ptr + idx_strct_ptr->tx_buff_write_index) = data;
		(idx_strct_ptr->tx_buff_write_index)++;
		stat = PASS;
	}
	return stat;
}

/****************************************************************************
Function:			uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data)
Input:				the interface where we want to read the data
Output:				FAIL if buffer is empty, PASS if some data is avaliable
PreCondition:		none
Overview:
****************************************************************************/
uint8_t getbyte(comm_interface_t comm_type, uint8_t * const data)
{
	uint8_t stat = FAIL;
	InterfaceSelection(comm_type);
	if(idx_strct_ptr->rx_buff_write_index != 0)
	{
		/* Buffer not empty */
		stat = PASS;
		*data = *(buff_rx_ptr + idx_strct_ptr->rx_buff_read_index);
		idx_strct_ptr->rx_buff_read_index++;
		if (idx_strct_ptr->rx_buff_read_index >= idx_strct_ptr->rx_buff_write_index)
		{
			if(comm_type != USBVCP)
			{
				idx_strct_ptr->rx_buff_write_index = 0;
				idx_strct_ptr->rx_buff_read_index  = 0;
			}
		}
	}
	return stat;
}

/****************************************************************************
Function:			uint16_t data_avail(comm_inerface_t comm_type)
Input:				the interface where we want to check for some avaliable data
Output:				the number of bytes avaliable in the read buffer of the interface
PreCondition:		none
Overview:
****************************************************************************/
uint16_t data_avail(comm_interface_t comm_type)
{
	InterfaceSelection(comm_type);
	return idx_strct_ptr->rx_buff_write_index;
}

/****************************************************************************
Function:			uint16_t data_toprocess(comm_inerface_t comm_type)
Input:				the interface where we want to check for some pending data
Output:				the number of bytes that need to be sended from the tx buffer
PreCondition:		none
Overview:
****************************************************************************/
uint16_t data_toprocess(comm_interface_t comm_type)
{
	InterfaceSelection(comm_type);
	return idx_strct_ptr->tx_buff_write_index;
}

/****************************************************************************
Function:			void clear_buff(comm_inerface_t comm_type)
Input:				the interface where we want to reset the tx and rx buffer
Output:				none
PreCondition:		none
Overview:			clear tx and rx buffer
****************************************************************************/
void clear_buff(comm_interface_t comm_type)
{
	InterfaceSelection(comm_type);
	idx_strct_ptr->rx_buff_write_index = 0;
	idx_strct_ptr->rx_buff_read_index  = 0;
	idx_strct_ptr->tx_buff_write_index = 0;
	idx_strct_ptr->tx_buff_read_index  = 0;
}

/****************************************************************************
Function:			void USB_set_rx_data_len(uint16_t len)
Input:				len of received data from vcp
Output:				none
PreCondition:		none
Overview:			none
****************************************************************************/
void USB_set_rx_data_len(uint16_t len)
{
	InterfaceSelection(USBVCP);
	idx_strct_ptr->rx_buff_write_index = len;
}

/****************************************************************************
Function:			void UsbPrintString(const char *buff, append_t character)
Input:				const char *buff, append_t character
Output:				none
PreCondition:		none
Overview:			stores the buffer in the vcp buffer
					appends some ascii character to perform user visualization
****************************************************************************/
void UsbPrintString(const char *buff, append_t character)
{
	while(*buff != '\0')
	{
		putbyte(USBVCP, *buff);
		buff++;
	}
	switch(character)
	{
		case APPEND_CR:
		putbyte(USBVCP, CR_);
		break;

		case APPEND_CRCR:
		putbyte(USBVCP, CR_);
		putbyte(USBVCP, CR_);
		case PRINT_ONLY:
		default:
		break;
	}
}

/****************************************************************************
Function:			uint16_t SetBuffer(comm_inerface_t comm_type)
Input:				comm_inerface_t comm_type
Output:				none
PreCondition:		none
Overview:			Copy the data from the vcp buffer to the selected interface buffer (UART/SPI/I2C)
****************************************************************************/
uint16_t SetBuffer(comm_interface_t comm_type)
{
	uint16_t data_len = 0;
	uint16_t i        = 0;
	data_len = data_avail(USBVCP);
	if(data_len)
	{
		for(i=0; i<data_len; i++)
			putbyte(comm_type, UserRxBufferFS[i]);
	}
	return data_len;
}

/****************************************************************************
Function:			void ToUsb(comm_inerface_t comm_type)
Input:				comm_inerface_t comm_type
Output:				none
PreCondition:		none
Overview:			creates a bridge between the UART/SPI/ or I2C buffer and the VCP BUFFER
					Starts the data transfer to the vcp
****************************************************************************/
void ToUsb(comm_interface_t comm_type)
{
	uint8_t byte;
	if(comm_type != USBVCP)
	{
		if( data_avail(comm_type) )
		{
			while( getbyte(comm_type, &byte) )
				putbyte(USBVCP, byte);
		}
	}
	if(usb_comm_type.tx_buff_write_index)
	{
		CDC_Transmit_FS(UserTxBufferFS, usb_comm_type.tx_buff_write_index);
		usb_comm_type.tx_buff_write_index = 0;
	}
}
