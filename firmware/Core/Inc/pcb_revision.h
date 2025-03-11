#ifndef INC_PCB_REVISION_H_
#define INC_PCB_REVISION_H_

/* Revision of printed circuit board */
#define PCB_REV1 1
#define PCB_REV2 2
#define PCB_REV PCB_REV2

/***************READ ME ******************/
/* CHANGES FROM REVISION 1 to REVISION 2 */
/*****************************************/

/* 1) Added external eeprom 24C64WP is SO8 package for data storage */
/* 2) USB LED and R2 have been moved from PB10 pin to PB12 pin */
/* 3) I2C LED and R3 have been moved from PB11 pin to PB13 pin */
/* 4) PB10 pin is configured for I2C SCL for 24C64 eeprom */
/* 5) PB11 pin is configured for I2C SDA for 24C64 eeprom */
/* 4) Added not buffered (NOT BUFFERED!!!!!) adc input to PC5 pin. The pin is reachable from extral connector */

#endif
