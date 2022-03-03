#include "ssd1306.h"
#include "gpio.h"

static uint8_t SSD1306_Buffer[SSD1306_BUFF_SIZE];
static SSD1306_t SSD1306;
static uint32_t ssd1306_time = 0;
static I2C_HandleTypeDef hi2c_ssd1306;

static i2c_error_t ssd1306_IicCheckStatus(uint32_t flag);

void ssd1306_Fill(SSD1306_COLOR color)
{
    uint32_t i;
    for(i = 0; i < SSD1306_BUFF_SIZE; i++)
    {
    	if(color == Black)
    		SSD1306_Buffer[i] = 0x00;
    	else
    		SSD1306_Buffer[i] = 0xFF;
    }
}

void ssd1306_UpdateScreen(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        ssd1306_WriteCommand(0xB0 + i);
        ssd1306_WriteCommand(0x00);
        ssd1306_WriteCommand(0x10);
        ssd1306_WriteBuff(&SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        /* Don't write outside the buffer */
        return;
    }

    /* Check if pixel should be inverted */
    if (SSD1306.Inverted)
    {
        color = (SSD1306_COLOR)!color;
    }

    /* Draw in the correct color */
    if (color == White)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color)
{
    uint32_t i, b, j;
    /* Check remaining space on current line */
    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font.FontWidth) ||
        SSD1306_HEIGHT <= (SSD1306.CurrentY + Font.FontHeight))
    {
        /* Not enough space on current line */
        return 0;
    }

    /* Translate font to screenbuffer */
    for (i = 0; i < Font.FontHeight; i++)
    {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for (j = 0; j < Font.FontWidth; j++)
        {
            if ((b << j) & 0x8000)
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            else
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);
        }
    }
    /* The current space is now taken */
    SSD1306.CurrentX += Font.FontWidth;
    /* Return written char for validation */
    return ch;
}

char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color)
{
    /* Write until null-byte */
    while (*str)
    {
        if (ssd1306_WriteChar(*str, Font, color) != *str)
        {
            /* Char could not be written */
            return *str;
        }
        /* Next char */
        str++;
    }
    /* Everything ok */
    return *str;
}

void ssd1306_InvertColors(void)
{
    SSD1306.Inverted = !SSD1306.Inverted;
}

void ssd1306_SetCursor(uint8_t x, uint8_t y)
{
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void ssd1306_iic_init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2cPinConfig(COMMUNICATION_MODE);
	hi2c_ssd1306.Instance             = I2C1;
	hi2c_ssd1306.Init.ClockSpeed      = 80000;
	hi2c_ssd1306.Init.DutyCycle       = I2C_DUTYCYCLE_2;
	hi2c_ssd1306.Init.OwnAddress1     = 0;
	hi2c_ssd1306.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c_ssd1306.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c_ssd1306.Init.OwnAddress2     = 0;
	hi2c_ssd1306.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c_ssd1306.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c_ssd1306);
}

uint8_t ssd1306_InitOled(void)
{
	i2c_error_t status = NO_ERROR;
    /* Wait for the screen to boot */
    HAL_Delay(100);
    status += ssd1306_WriteCommand(0xAE);   /* Display off */
    status += ssd1306_WriteCommand(0x20);   /* Set Memory Addressing Mode */
    status += ssd1306_WriteCommand(0x10);   /* 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid */
    status += ssd1306_WriteCommand(0xB0);   /* Set Page Start Address for Page Addressing Mode,0-7 */
    status += ssd1306_WriteCommand(0xC8);   /* Set COM Output Scan Direction */
    status += ssd1306_WriteCommand(0x00);   /* Set low column address */
    status += ssd1306_WriteCommand(0x10);   /* Set high column address */
    status += ssd1306_WriteCommand(0x40);   /* Set start line address */
    status += ssd1306_WriteCommand(0x81);   /* set contrast control register */
    status += ssd1306_WriteCommand(0xFF);
    status += ssd1306_WriteCommand(0xA1);   /* Set segment re-map 0 to 127 */
    status += ssd1306_WriteCommand(0xA6);   /* Set normal display */
    status += ssd1306_WriteCommand(0xA8);   /* Set multiplex ratio(1 to 64) */
    status += ssd1306_WriteCommand(SSD1306_HEIGHT - 1);
    status += ssd1306_WriteCommand(0xA4);   /* 0xa4,Output follows RAM content;0xa5,Output ignores RAM content */
    status += ssd1306_WriteCommand(0xD3);   /* Set display offset */
    status += ssd1306_WriteCommand(0x00);   /* No offset */
    status += ssd1306_WriteCommand(0xD5);   /* Set display clock divide ratio/oscillator frequency */
    status += ssd1306_WriteCommand(0xF0);   /* Set divide ratio */
    status += ssd1306_WriteCommand(0xD9);   /* Set pre-charge period */
    status += ssd1306_WriteCommand(0x22);
    status += ssd1306_WriteCommand(0xDA);   /* Set com pins hardware configuration */
    status += ssd1306_WriteCommand(0x12);   /* Do not use COM left/right remap */
    status += ssd1306_WriteCommand(0xDB);   /* Set vcomh */
    status += ssd1306_WriteCommand(0x20);   /* 0x20,0.77xVcc */
    status += ssd1306_WriteCommand(0x8D);   /* Set DC-DC enable */
    status += ssd1306_WriteCommand(0x14);
    status += ssd1306_WriteCommand(0xAF);   /* Turn on SSD1306 panel */
    // Clear screen
    ssd1306_Fill(Black);
    // Flush buffer to screen
    ssd1306_UpdateScreen();
    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
    SSD1306.Initialized = 1;
    return status;
}

void ssd1306DemoApp(void)
{
	ssd1306_iic_init();
	HAL_Delay(1000);
	ssd1306_InitOled();
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("SSD1306", Font_11x18, White);
	ssd1306_SetCursor(0, 36);
	ssd1306_WriteString("STM32", Font_11x18, White);
	ssd1306_UpdateScreen();
}

i2c_error_t ssd1306_WriteCommand(uint8_t data)
{
	i2c_error_t stat = NO_ERROR;

	ssd1306_time = HAL_GetTick();

	/* check for peripheral activation */
	if((hi2c_ssd1306.Instance->CR1 & I2C_CR1_PE) == 0 )
		stat = I2C_NOT_ACTIVATED;

	hi2c_ssd1306.Instance->CR1 |= I2C_CR1_START;

	/* generation of start condition */
	stat = ssd1306_IicCheckStatus(I2C_SR1_SB);
	if(stat)
		return stat;

	/*selecting type of transmission read/write */
	hi2c_ssd1306.Instance->DR = SSD1306_IIC_ADDRESS&0xFE;

	/* send address and wait for complete address transfer and ack */
	stat = ssd1306_IicCheckStatus(I2C_SR1_ADDR);
	if(stat)
		return stat;
	hi2c_ssd1306.Instance->SR1;
	hi2c_ssd1306.Instance->SR2;

	/* check for transmitter register empty */
	stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
	if(stat)
		return stat;

	/* send command */
	hi2c_ssd1306.Instance->DR  = SSD1306_CMD_ONE_COMMAND;
	stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
	if(stat)
		return stat;
	/* send data and stop condition */
	hi2c_ssd1306.Instance->DR  = data;
	stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
	if(stat)
		return stat;

	hi2c_ssd1306.Instance->CR1 |= I2C_CR1_STOP;

	HAL_Delay(10);
	return stat;
}

i2c_error_t ssd1306_WriteBuff(const uint8_t *buff, uint16_t len)
{
	uint16_t i;
	i2c_error_t stat = NO_ERROR;

	ssd1306_time = HAL_GetTick();

	/* check for peripheral activation */
	if((hi2c_ssd1306.Instance->CR1 & I2C_CR1_PE) == 0 )
		stat = I2C_NOT_ACTIVATED;

	hi2c_ssd1306.Instance->CR1 |= I2C_CR1_START;

	/* generation of start condition */
	stat = ssd1306_IicCheckStatus(I2C_SR1_SB);
	if(stat)
		return stat;

	/*selecting type of transmission read/write */
	hi2c_ssd1306.Instance->DR = SSD1306_IIC_ADDRESS&0xFE;

	/* send address and wait for complete address transfer and ack */
	stat = ssd1306_IicCheckStatus(I2C_SR1_ADDR);
	if(stat)
		return stat;
	hi2c_ssd1306.Instance->SR1;
	hi2c_ssd1306.Instance->SR2;

	/* check for transmitter register empty */
	stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
	if(stat)
		return stat;

	hi2c_ssd1306.Instance->DR  = 0x40;
	stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
	if(stat)
		return stat;

	/* send data and stop condition */
	for(i = 0; i< len; i++)
	{
		hi2c_ssd1306.Instance->DR  = *buff;
		buff++;
		stat = ssd1306_IicCheckStatus(I2C_SR1_TXE);
		if(stat)
			return stat;
	}
	hi2c_ssd1306.Instance->CR1 |= I2C_CR1_STOP;

	HAL_Delay(10);

	return stat;
}

static i2c_error_t ssd1306_IicCheckStatus(uint32_t flag)
{
	while ((hi2c_ssd1306.Instance->SR1 & flag) == 0)
	{
		if ((HAL_GetTick() - ssd1306_time ) >= IIC_TIMEOUT)
			return I2C_TIMEOUT;

		if ((hi2c_ssd1306.Instance->SR1 & I2C_SR1_OVR) != 0)
			return I2C_OVERRUN_UNDERRUN;

		/* Set when no acknowledge is returned*/
		if ((hi2c_ssd1306.Instance->SR1 & I2C_SR1_AF) != 0)
			return I2C_ACKNOWLEDGE_FAILURE;

		/*Arbitration Lost - another master on the bus*/
		if ((hi2c_ssd1306.Instance->SR1 & I2C_SR1_ARLO) != 0)
			return I2C_ARBITRATION_LOST;

		if ((hi2c_ssd1306.Instance->SR1 & I2C_SR1_BERR) != 0)
			return I2C_BUS_ERROR;
	}
	return NO_ERROR;
}
