#ifndef MIPOT_32001270_32001534_32001279_32001534_H_
#define MIPOT_32001270_32001534_32001279_32001534_H_

/* DOC reference https://www.mipot.com/wp-content/uploads/2021/06/DualMode_TRX_Command_Reference_rev1.6.pdf */

#define MIPTRX_PROGRAMMING_TIMEOUT     50 /* ms */
#define MIPTRX_START_BYTE              0x18
#define MIPTRX_STANDART_CMD_LEN        0x05
#define MIPTRX_CHANNEL1_CMD_BYTE       0x50
#define MIPTRX_CHANNEL2_CMD_BYTE       0x51
#define MIPTRX_MODULATION_CMD_BYTE     0x52
#define MIPTRX_SYNC_CMD_BYTE           0x53
#define MIPTRX_POWER_CMD_BYTE          0x01
#define MIPTRX_RSSI_CMD_BYTE           0x55
#define MIPTRX_DEFAULTSETTING_CMD_BYTE 0x0A

#define MIPTRX_CHANNEL_FREQ_868_1   0x00 /* Set TRX CHX to 868.1 MHz */
#define MIPTRX_CHANNEL_FREQ_868_15  0x00 /* Set TRX CHX to 868.15 MHz */
#define MIPTRX_CHANNEL_FREQ_868_2   0x00 /* Set TRX CHX to 868.2 MHz */
#define MIPTRX_CHANNEL_FREQ_868_3   0x00 /* Set TRX CHX to 868.3 MHz */
#define MIPTRX_CHANNEL_FREQ_868_4   0x00 /* Set TRX CHX to 868.4 MHz */
#define MIPTRX_CHANNEL_FREQ_868_45  0x00 /* Set TRX CHX to 868.45 MHz */
#define MIPTRX_CHANNEL_FREQ_868_5   0x00 /* Set TRX CHX to 868.5 MHz */
#define MIPTRX_CHANNEL_FREQ_868_825 0x00 /* Set TRX CHX to 868.825 MHz */
#define MIPTRX_CHANNEL_FREQ_869_075 0x00 /* Set TRX CHX to 869.075 MHz */
#define MIPTRX_CHANNEL_FREQ_869_85  0x00 /* Set TRX CHX to 868.85 MHz */

#define MIPTRX_MODULATION_TYPE_OOK  0x00 /* trx modulation OOK */
#define MIPTRX_MODULATION_TYPE_2SFK 0x01 /* trx modulation 2FSK */

#define MIPTRX_SYNC_MODE_DISABLED        0x00 /*sync disabled, normal mode, data trasparent */
#define MIPTRX_SYNC_MODE_SLOW            0x01 /*sync disabled, "slow" mode */
#define MIPTRX_SYNC_MODE_BAUDRATE_1200   0x02 /*sync enabled, baud rate 1200 */
#define MIPTRX_SYNC_MODE_BAUDRATE_2400   0x03 /*sync enabled, baud rate 2400 */
#define MIPTRX_SYNC_MODE_BAUDRATE_4800   0x04 /*sync enabled, baud rate 4800 */
#define MIPTRX_SYNC_MODE_BAUDRATE_9600   0x05 /*sync enabled, baud rate 9600 */
#define MIPTRX_SYNC_MODE_BAUDRATE_19200  0x06 /*sync enabled, baud rate 19200 */
#define MIPTRX_SYNC_MODE_BAUDRATE_384000 0x07 /*sync enabled, baud rate 38400 */

#define MIPTRX_POWER_MODE_10dbm 0x00 /* set TRX output power to +10 dbm */
#define MIPTRX_POWER_MODE_7dbm  0x01 /* set TRX output power to +7 dbm */
#define MIPTRX_POWER_MODE_4dbm  0x02 /* set TRX output power to +4 dbm */
#define MIPTRX_POWER_MODE_1dbm  0x03 /* set TRX output power to +1 dbm */
#define MIPTRX_POWER_MODE__2dbm 0x04 /* set TRX output power to -2 dbm */
#define MIPTRX_POWER_MODE__5dbm 0x05 /* set TRX output power to -5 dbm */

typedef enum{
	MODULE_PROGRAMMING_FAIL = 0u,
	MODULE_PROGRAMMING_SUCCESS,
}MoudleProgramResult_t;

MoudleProgramResult_t ProgramTRXModule(void);

#endif
