#ifndef INC_UTILITY_H_
#define INC_UTILITY_H_

#include <stdint.h>

#define EEPROM_UPTIME_STORE_TIME 30000

#define DAY_IS_SECONDS  86400
#define HOUR_IS_SECONDS 3600
#define MIN_IS_SECONDS   60

typedef struct{
	uint8_t sec;
	uint8_t min;
	uint8_t hours;
	uint16_t days;
}uptime_time_t;

typedef enum
{
	UPTIME_READ_DATA = 0,
	UPTIME_GET_TICK,
	UPTIME_CHECK_AND_STORE,
	UPTIME_EEPROM_ERR
}utility_uptime_s;

utility_uptime_s UtilityUptime(void);
uptime_time_t UptimeGet(void);

#endif
