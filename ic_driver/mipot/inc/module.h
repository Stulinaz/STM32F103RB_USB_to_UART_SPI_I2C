#ifndef MIPOT_32001269_32001534_INC_MODULE_H_
#define MIPOT_32001269_32001534_INC_MODULE_H_

#include "32001269_32001534.h"
#include "32001270_32001535.h"

typedef enum{
	MODULE_PROGRAMMING_FAIL = 0u,
	MODULE_PROGRAMMING_SUCCESS,
}MoudleProgramResult_t;

MoudleProgramResult_t ProgramTRXModule(uint8_t *buff, uint8_t len);

#endif
