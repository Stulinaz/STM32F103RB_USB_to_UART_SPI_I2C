#ifndef INC_COMMAND_PROCESS_H_
#define INC_COMMAND_PROCESS_H_

#include "main.h"
#include "command_list.h"

#define MIN_CMD_LEN           0x05
#define CHAR_SPACE            0x20
#define TEMP_BUFF_DIM         50

command_t Decode  (uint16_t cmd_len, uint8_t *const param);
uint16_t DecToChar(uint8_t value);
uint8_t ParseCommand(uint16_t len, uint8_t *buff);

#endif
