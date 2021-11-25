#ifndef INC_COMMAND_LIST_H_
#define INC_COMMAND_LIST_H_

typedef enum{
	NO_COMMAND = 0u,
	UNKNOWN_COMMAND ,
	USER_TRANSFER_REQUEST,
	USER_SERIAL_INTERFACE_SELECTED,
	USER_I2C_INTERFACE_SELECTED,
	USER_SPI_INTERFACE_SELECTED,
	USER_CH1_ACTIVATED,
	USER_CH1_DEACTIVATE,
	USER_CH2_ACTIVATED,
	USER_CH2_DEACTIVATE,
	USER_TEST_ONBOARD_LED,
	USER_CLOSE_INTERFACE,
	USER_SER_SET_BAUDRATE,
	USER_SPI_CLK_POLARITY_LOW,
	USER_SPI_CLK_POLARITY_HIGH,
	USER_SPI_SPEED_SELECTION,
	USER_SPI_NSS_ACTIVE_LOW,
	USER_SPI_NSS_ACTIVE_HIGH,
	USER_I2C_SET_SLAVE_ADDRESS,
	USER_IC2_ACCESS_MODE_TRANSMITTER,
	USER_IC2_ACCESS_MODE_RECEIVER,
	USER_I2C_WRITE_DATA,
	USER_I2C_READ_DATA,
	USER_I2C_REPEAT_START,
	USER_I2C_SEND_ADDRESS_READ,
	USER_I2C_REQUEST_QUEUE_DATA,
	USER_I2C_REQUEST_QUEUE_DELETE,
	HELP
}command_t;

extern const char serial_cmd_select[];
extern const char i2c_cmd_select[];
extern const char spi_cmd_select[];
extern const char help[];
extern const char ch1_on[];
extern const char ch1_off[];
extern const char ch2_on[];
extern const char ch2_off[];
extern const char test_led[];
extern const char close_interface[];
extern const char serial_bausel[];
extern const char spi_clkpollow[];
extern const char spi_clkpolhigh[];
extern const char spi_speed[];
extern const char spi_nssactivehigh[];
extern const char spi_nssactivelow[];
extern const char i2c_address[];
extern const char i2c_transmitter[];
extern const char i2c_receiver[];
extern const char i2c_access_write[];
extern const char i2c_access_read[];
extern const char i2c_repeat_start[];
extern const char i2c_queue_req[];
extern const char i2c_queue_delete[];
extern const char transfer_start[];
extern const char queue_full[];
extern const char wrong_param[];
extern const char ready[];
#endif
