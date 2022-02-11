#include "sys_definitions.h"
#include "serial.h"
#include "gpio.h"
#include "tim4_1us_tick.h"
#include "buffers_manager.h"
#include "main.h"
#include "module.h"

static void MipotTRXUartInit(void);
static void MipotTRXUartStart(void);

UART_HandleTypeDef mipottrxhuart2;

/* DOC REFERENCE */
/* https://www.mipot.com/wp-content/uploads/2021/06/DualMode_TRX_Command_Reference_rev1.6.pdf */
/* https://www.mipot.com/wp-content/uploads/2021/06/32001270_Datasheet_rev1.5.pdf */


/* HARDWARE CONFIGURATION */
/* TRX CH_SEL PIN -> STM32_MULTIPROTOCOL UART TX  */
/* TRX EN PIN     -> STM32_MULTIPROTOCOL UART RTS */
/* TRX TX/RX PIN  -> STM32_MULTIPROTOCOL UART CTS */

static void MipotTRXUartInit(void)
{
	mipottrxhuart2.Instance          = USART2;
	mipottrxhuart2.Init.BaudRate     = SER_BAUDRATE_DEFAULT;
	mipottrxhuart2.Init.WordLength   = UART_WORDLENGTH_8B;
	mipottrxhuart2.Init.StopBits     = UART_STOPBITS_1;
	mipottrxhuart2.Init.Parity       = UART_PARITY_NONE;
	mipottrxhuart2.Init.Mode         = UART_MODE_TX_RX;
	mipottrxhuart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	mipottrxhuart2.Init.OverSampling = UART_OVERSAMPLING_16;
	mipottrxhuart2.Init.BaudRate     = 9600;
}

static void MipotTRXUartStart(void)
{
	RCC->APB1ENR |= 0x00020000;
	ser_comm_type.tx_buff_read_index  = 0;
	ser_comm_type.tx_buff_read_index  = 0;
	ser_comm_type.rx_buff_write_index = 0;
	ser_comm_type.tx_buff_read_index  = 0;
	//HAL_UART_DeInit(&mipottrxhuart2);
	MipotTRXUartInit();
	SerialPinConfig(COMMUNICATION_MODE);
	HAL_UART_Init(&mipottrxhuart2);
	__HAL_UART_ENABLE_IT(&mipottrxhuart2,UART_IT_RXNE);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

MoudleProgramResult_t ProgramTRXModule(uint8_t *buff, uint8_t len)
{
	uint8_t cmd[MIP1534TRX_STANDART_CMD_LEN];
	uint8_t i     = 0;
	uint8_t crc   = 0;
	uint32_t time = 0;
	MoudleProgramResult_t stat = MODULE_PROGRAMMING_FAIL;

	/* CRC IS NOT PART OF THE BUFFER */
	if( len >= MIP1534TRX_STANDART_CMD_LEN)
		return stat;

	for(i=0;i<len;i++)
	{
		cmd[i] = buff[i];
		crc = crc^cmd[i];
	}

	/* STORE CRC */
	cmd[i] = 0xFF - crc;

	/* Output the command on CH_SEL pin*/
	for(i=0; i<len+1; i++)
		putbyte(UART, cmd[i]);

	HAL_Delay(100);

	/* Generate a pulse on EN pin according to reference manual */
	HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_SET);
	TIM4_delay_us(80);
	HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_RESET);

	HAL_Delay(10);

	SerStartTransmit();

	time = HAL_GetTick();

	/* Check for module feedback */
	while(1)
	{
		/* Check detection of low impulse on TX/RX pin */
		if( HAL_GPIO_ReadPin(UART_CTS_PORT, UART_CTS_PIN) == GPIO_PIN_RESET)
		{
			stat = MODULE_PROGRAMMING_SUCCESS;
			break;
		}
		if( (HAL_GetTick() - time) >= MIP1534TRX_PROGRAMMING_TIMEOUT)
			break;
	}
	return stat;
}
