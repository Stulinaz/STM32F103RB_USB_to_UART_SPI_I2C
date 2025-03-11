#ifndef INC_AT_COMMANDS_UTILITY_H_
#define INC_AT_COMMANDS_UTILITY_H_

#include <stdint.h>

typedef enum{
    STRING_NOT_DETECTED = 0,
    STRING_DETECTED
}search_result_t;

search_result_t StringSearch(const char *full, const char *search, uint16_t *const pos_start, uint16_t *const pos_end);

#endif
