#ifndef INC_APP_USB_H_
#define INC_APP_USB_H_

#include "main.h"

#define INACTIVITY_TIMEOUT (uint32_t) 50000
#define CONNECTION_TIMEOUT (uint32_t) 5000
#define USB_RESET_TIMEOUT  (uint32_t) 200
#define MAN_TIMEOUT        (uint32_t) 50
#define CONNECTED          1
#define DISCONNECTED       0

typedef enum
{
	CHECK_USB = 0u,
	RESET_USB,
	NO_DATA_TO_PROCESS,
	CHECK_FOR_INACTIVITY,
	WAIT_FOR_DATA,
	DECODE_MESSAGE,
	RESTART,
	USB_FAIL
}command_manager_s;

void DecodeFromPc(void);

#endif
