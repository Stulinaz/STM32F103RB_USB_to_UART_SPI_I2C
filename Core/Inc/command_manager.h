#ifndef INC_COMMAND_MANAGER_H_
#define INC_COMMAND_MANAGER_H_

#include "main.h"

#define INACTIVITY_TIMEOUT (uint32_t) 50000
#define CONNECTION_TIMEOUT (uint32_t) 5000
#define MAN_TIMEOUT        (uint32_t) 50
#define CONNECTED          1
#define DISCONNECTED       0

typedef enum
{
	CHECK_USB = 0u,
	NO_DATA_TO_PROCESS,
	CHECK_FOR_INACTIVITY,
	WAIT_FOR_DATA,
	DECODE_MESSAGE,
	RESTART
}command_manager_s;

void DecodeFromPc(void);

#endif
