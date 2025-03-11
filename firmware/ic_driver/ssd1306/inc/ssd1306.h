#ifndef SSD1306_INC_SSD1306_H_
#define SSD1306_INC_SSD1306_H_

#include <stdint.h>
#include "i2c_callback.h"
#include "fonts.h"

/*****************REFERENCE DOCUMENTATIONS AND GIT*******************/
//https://www.codeinsideout.com/blog/stm32/oled-ssd1306/
//https://iotexpert.com/debugging-ssd1306-display-problems/
//https://github.com/4ilo/ssd1306-stm32HAL/blob/master/lib/ssd1306.h

/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

#define SSD1306_BUFF_SIZE ((SSD1306_WIDTH * SSD1306_HEIGHT) / 8)

#define SSD1306_IIC_ADDRESS_TYPE1 0x78u
#define SSD1306_IIC_ADDRESS_TYPE2 0x79u
#define SSD1306_IIC_ADDRESS SSD1306_IIC_ADDRESS_TYPE1

/* DOT MATRIX */
#define SSD1306_DISPALY_DOT_ROW 128 /* 16 bytes */
#define SSD1306_DISPALY_DOT_COL 64  /*  8 bytes */

/* CONTROL BYTE */
#define SSD1306_CONTROL_BYTE_MSK  0xC0u
#define SSD1306_CONTROL_BYTE_Co   (0x80u & SSD1306_CONTROL_BYTE_MSK)
#define SSD1306_CONTROL_BYTE_DC   (0x40u & SSD1306_CONTROL_BYTE_MSK)
#define SSD1306_CMD_MULTIPLE_COMMANDS SSD1306_CONTROL_BYTE_Co
#define SSD1306_CMD_ONE_COMMAND 0x00
#define SSD1306_CMD_MULTIPLE_DATA SSD1306_CONTROL_BYTE_Co | SSD1306_CONTROL_BYTE_DC
#define SSD1306_CMD_ONE_DATA SSD1306_CONTROL_BYTE_DC

/* COMMAND TABLE */
/* POWER MODE */
#define SSD1306_DISPLAY_MODE_ON   0xAFu /* Display OFF (sleep mode) (RESET) */
#define SSD1306_DISPLAY_MODE_OFF  0xAEu /* Display ON in normal mode */

/* FETCH FROM RAM CONTENT */
#define SSD1306_DISPLAY_MODE_RAM_CONTENT   0xA4u /* Outputs follows RAM content */
#define SSD1306_DISPLAY_MODE_RAM_IGNORED   0xA5u /* Outputs ignores RAM content */

/* SET NORMAL/INVERSE DISPLAY*/
#define SSD1306_DISPLAY_MODE_NORMAL    0xA6u /* 0 in RAM: off in display panel - 1 in RAM: ON in diplay panel */
#define SSD1306_DISPLAY_MODE_INVERSE   0xA7u /* 1 in RAM: off in display panel - 0 in RAM: ON in diplay panel */

/* ADDRESSING SETTINGS COMMAND TABLE */
#define SSD1306_PAGE_ADDR_CMD       0x22u
#define SSD1306_PAGE_START_ADDR_MSK 0x07u /* Page start Address, range :0-7d, (RESET = 0d) */
#define SSD1306_PAGE_END_ADDR_MSK   0x07u /* Page end Address,   range :0-7d, (RESET = 7d) */

/* NOP */
#define SSD1306_NOP_CMD 0xE3u /* COmmand for no operation */

#define SSD1306_COM_LR_REMAP

/* Enumeration for screen colors */
typedef enum {
    Black = 0x00,   // Black color, no pixel
    White = 0x01,   // Pixel is set. Color depends on LCD
} SSD1306_COLOR;


/*  Struct to store transformations */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

void ssd1306_Fill(SSD1306_COLOR color);
void ssd1306_UpdateScreen(void);
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char ssd1306_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char ssd1306_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void ssd1306_InvertColors(void);
void ssd1306_SetCursor(uint8_t x, uint8_t y);
void ssd1306_iic_init(void);
uint8_t ssd1306_InitOled(void);
void ssd1306DemoApp(void);
i2c_error_t ssd1306_WriteCommand(uint8_t data);
i2c_error_t ssd1306_WriteBuff(const uint8_t *buff, uint16_t len);
#endif
