#include "buffers_manager.h"
#include "sys_definitions.h"

uint8_t ser_tx_buff[SERIAL_TX_BUFF_DIM];
uint8_t ser_rx_buff[SERIAL_RX_BUFF_DIM];

uint8_t i2c_tx_buff[I2C_TX_BUFF_DIM];
uint8_t i2c_rx_buff[I2C_RX_BUFF_DIM];

uint8_t spi_tx_buff[SPI_TX_BUFF_DIM];
uint8_t spi_rx_buff[SPI_RX_BUFF_DIM];

static void interface_selection (comm_inerface_t type_t);

volatile comm_index_t ser_comm_type = { 0, 0, 0, 0, SERIAL_TX_BUFF_DIM, SERIAL_RX_BUFF_DIM };
volatile comm_index_t spi_comm_type = { 0, 0, 0, 0, SPI_TX_BUFF_DIM, SPI_RX_BUFF_DIM};
volatile comm_index_t i2c_comm_type = { 0, 0, 0, 0,  I2C_TX_BUFF_DIM, I2C_RX_BUFF_DIM};
volatile comm_index_t usb_comm_type = { 0, 0, 0, 0,  APP_TX_DATA_SIZE, APP_RX_DATA_SIZE};

volatile comm_index_t *idx_strct_ptr;
volatile uint8_t      *buff_tx_ptr;
volatile uint8_t      *buff_rx_ptr;

static void interface_selection (volatile comm_inerface_t type_t)
{
	switch(type_t)
	{
		case SER_INTERFACE:
		{
			idx_strct_ptr = &ser_comm_type;
			buff_tx_ptr   = ser_tx_buff;
			buff_rx_ptr   = ser_rx_buff;
			break;
		}
		case I2C_INTERFACE:
		{
			idx_strct_ptr = &i2c_comm_type;
			buff_tx_ptr   = i2c_tx_buff;
			buff_rx_ptr   = i2c_rx_buff;
			break;
		}
		case SPI_INTERFACE:
		{
			idx_strct_ptr = &spi_comm_type;
			buff_tx_ptr   = spi_tx_buff;
			buff_rx_ptr   = spi_rx_buff;
			break;
		}
		case USB_INTERFACE:
		{
			idx_strct_ptr = &usb_comm_type;
			buff_tx_ptr   = UserTxBufferFS;
			buff_rx_ptr   = UserRxBufferFS;
			break;
		}
		default:
			break;
	}
}

uint8_t putbyte(comm_inerface_t comm_type, uint8_t data)
{
	uint8_t stat = FAIL;
	interface_selection (comm_type);
	if (idx_strct_ptr->tx_buff_write_index < (idx_strct_ptr->tx_max_buff_dim))
	{
		*(buff_tx_ptr + idx_strct_ptr->tx_buff_write_index) = data;
		(idx_strct_ptr->tx_buff_write_index)++;
		stat = PASS;
	}
	return stat;
}

uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data)
{
	uint8_t stat = FAIL;
	interface_selection (comm_type);
	if(idx_strct_ptr->rx_buff_write_index != 0)
	{
		/*buffer not empty*/
		stat = PASS;
		*data = *(buff_rx_ptr + idx_strct_ptr->rx_buff_read_index);
		idx_strct_ptr->rx_buff_read_index++;
		if (idx_strct_ptr->rx_buff_read_index >= idx_strct_ptr->rx_buff_write_index)
		{
			if(comm_type != USB_INTERFACE)
			{
				idx_strct_ptr->rx_buff_write_index = 0;
				idx_strct_ptr->rx_buff_read_index  = 0;
			}
		}
	}
	return stat;
}

uint16_t data_avail(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	return idx_strct_ptr->rx_buff_write_index;
}

uint16_t data_toprocess(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	return  idx_strct_ptr->tx_buff_write_index;
}

void clear_buff(comm_inerface_t comm_type)
{
	interface_selection (comm_type);
	idx_strct_ptr->rx_buff_write_index = 0;
	idx_strct_ptr->rx_buff_read_index  = 0;
}

void USB_set_rx_data_len(uint16_t len)
{
	interface_selection (USB_INTERFACE);
	idx_strct_ptr->rx_buff_write_index = len;
}

void UsbPrintString(const char *buff, append_t character)
{
	if(usb_comm_type.tx_buff_write_index == 0)
	{
		putbyte(USB_INTERFACE, '[');
		putbyte(USB_INTERFACE, 'S');
		putbyte(USB_INTERFACE, 'T');
		putbyte(USB_INTERFACE, 'M');
		putbyte(USB_INTERFACE, ']');
		putbyte(USB_INTERFACE, ' ');
	}
	while(*buff != '\0')
	{
		putbyte(USB_INTERFACE, *buff);
		buff++;
	}
	switch(character)
	{
		case APPEND_CR:
		putbyte(USB_INTERFACE, CR_);
		break;

		case APPEND_CRCR:
		putbyte(USB_INTERFACE, CR_);
		putbyte(USB_INTERFACE, CR_);
		case PRINT_ONLY:
		default:
		break;
	}
	CDC_Transmit_FS(UserTxBufferFS, usb_comm_type.tx_buff_write_index);
	usb_comm_type.tx_buff_write_index = 0;
}

uint16_t SetBuffer(comm_inerface_t comm_type)
{
	uint16_t data_len, i;
	data_len = data_avail(USB_INTERFACE);
	if(data_len)
	{
		for(i=0; i<data_len; i++)
			putbyte(comm_type,UserRxBufferFS[i]);
	}
	return data_len;
}

void ToUsb(comm_inerface_t comm_type)
{
	uint8_t byte;
	if( data_avail(comm_type) )
	{
		while( getbyte(comm_type, &byte) )
			putbyte(USB_INTERFACE, byte);
		CDC_Transmit_FS(UserTxBufferFS, usb_comm_type.tx_buff_write_index);
		usb_comm_type.tx_buff_write_index = 0;
	}
}
