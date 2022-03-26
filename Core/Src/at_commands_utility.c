#include "at_commands_utility.h"
#include <string.h>

/****************************************************************************
Function:			search_result_t StringSearch(const char *full, const char *search, uint16_t *const pos_start, uint16_t *const pos_end)
Input:				uint8_t *full(pointer to the main buffer)
				    uint8_t *search(pointer to string that is need to be searched in the full string)
Output:             <STRING_NOT_DETECTED> -> if the <search> string is not present in the <full> string
					<STRING_DETECTED>     -> if the <search> string has been found in the <full> string
					<pos_start>           -> index of <full> string when the match begins
					<pos_end>             -> index of <full> string when the match ends
PreCondition:		None
Overview:			none
****************************************************************************/
search_result_t StringSearch(const char *full, const char *search, uint16_t *const pos_start, uint16_t *const pos_end)
{
    uint16_t i;
    uint16_t len_full      = 0;
    uint16_t len_search    = 0;
    uint16_t token_search  = 0;
    search_result_t status = STRING_NOT_DETECTED;
    *pos_start = 0;
    *pos_end   = 0;
    len_full   = strlen(full);
    len_search = strlen(search);
    if (len_search == len_full)
    {
        status = STRING_DETECTED;
        for(i=0; i<len_full; i++)
        {
            if(full[i] != search[i])
                status = STRING_NOT_DETECTED;
        }
        if (status == STRING_DETECTED)
        	*pos_end = len_full-1;
    }
    else if (len_full > len_search)
    {
        token_search = 0;
        for( i=0; i<len_full; i++)
        {
            if( full[i] != search[token_search])
            {
                token_search = 0;
                if (full[i] == search[token_search])
                	token_search++;
            }
            else
                token_search++;

            if (token_search >= len_search)
            {
            	*pos_start = (i-len_search)+1;
            	*pos_end   = i;
                status     = STRING_DETECTED;
                break;
            }
        }
    }
    else
    {
        /* len_full is smaller than search string*/
        ;
    }
    return status;
}

