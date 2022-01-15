#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"
#include "command_process.h"
#include "command_list.h"

void Application (command_t * user_cmd, uint8_t *user_data, uint8_t user_data_len);
void TransferToPc(void);
#endif
