#include "command_list.h"

/*general commands */
const char serial_cmd_select[] = "stm-s";
const char i2c_cmd_select[]    = "stm-i";
const char spi_cmd_select[]    = "stm-p";
const char help[]              = "stm-h";
const char ch1_on[]            = "stm-ch1on";
const char ch1_off[]           = "stm-ch1off";
const char ch2_on[]            = "stm-ch2on";
const char ch2_off[]           = "stm-ch2off";

/*ser command*/
const char serial_bausel[]     = "ser-b";

/* spi commands */
const char spi_clkpollow[]     = "spi-cl";
const char spi_clkpolhigh[]    = "spi-ch";
const char spi_speed[]         = "spi-s";
const char spi_nssactivehigh[] = "spi-nssh";
const char spi_nssactivelow[]  = "spi-nssl";

/*i2c commands */
const char i2c_address[]       = "i2c-a0x";
const char i2c_transmitter[]   = "i2c-mt";
const char i2c_receiver[]      = "i2c-mr";
const char i2c_access_write[]  = "i2c-w0x";
const char i2c_access_read[]   = "i2c-r";
const char i2c_repeat_start[]  = "i2c-s";
const char i2c_queue_req[]     = "i2c-q";
const char i2c_queue_delete[]  = "i2c-d";
const char transfer_start[]    = "i2c-x";

/* accessories */
const char queue_full[]        = "Queue full";
const char wrong_param[]       = "Wrong parameter";
const char ready[]             = "Ready";
