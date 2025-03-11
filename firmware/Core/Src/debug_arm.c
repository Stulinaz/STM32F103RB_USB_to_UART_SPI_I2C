#include "debug_arm.h"
#include "main.h"

void SWO_PrintString(const char *s)
{
  while (*s!='\0') {
	  ITM_SendChar(*s++);
  }
}

void SetSWbreakpoint(void)
{
	asm("BKPT 255");
}
