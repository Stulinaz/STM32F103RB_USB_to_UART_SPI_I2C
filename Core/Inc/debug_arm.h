#ifndef INC_DEBUG_ARM_H_
#define INC_DEBUG_ARM_H_

#define CORE_M3

#ifdef CORE_M3
#define ITM32(n) (*((volatile unsigned long*)(0xE0000000 + 4*n)))
#endif

void SWO_PrintString(const char *s);
void SetSWbreakpoint(void);

#endif
