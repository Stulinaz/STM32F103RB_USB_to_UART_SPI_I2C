#ifndef MIPOT32001270_32001535_H_
#define MIPOT32001270_32001535_H_

#define MIP1535TRX_PROGRAMMING_TIMEOUT       50 /* ms */
#define MIP1535TRX_START_BYTE                0x18
#define MIP1535TRX_STANDART_CMD_LEN          0x05
#define MIP1535TRX_CHANNEL1_CMD_BYTE         0x50
#define MIP1535TRX_CHANNEL2_CMD_BYTE         0x51
#define MIP1535TRX_MODULATION_CMD_BYTE       0x52
#define MIP1535TRX_SYNC_CMD_BYTE             0x53
#define MIP1535TRX_POWER_CMD_BYTE            0x01
#define MIP1535TRX_RSSI_CMD_BYTE             0x55
#define MIP1535TRX_DEFAULTSETTING_CMD_BYTE   0x0A

#define MIP1535TRX_CHANNEL_FREQ_868_1        0x00 /* Set TRX CHX to 868.1 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_15       0x01 /* Set TRX CHX to 868.15 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_2        0x02 /* Set TRX CHX to 868.2 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_3        0x03 /* Set TRX CHX to 868.3 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_4        0x04 /* Set TRX CHX to 868.4 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_45       0x05 /* Set TRX CHX to 868.45 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_5        0x06 /* Set TRX CHX to 868.5 MHz */
#define MIP1535TRX_CHANNEL_FREQ_868_825      0x07 /* Set TRX CHX to 868.825 MHz */
#define MIP1535TRX_CHANNEL_FREQ_869_075      0x08 /* Set TRX CHX to 869.075 MHz */
#define MIP1535TRX_CHANNEL_FREQ_869_85       0x09 /* Set TRX CHX to 868.85 MHz */

#define MIP1535TRX_SYNC_MODE_DISABLED        0x00 /*sync disabled, normal mode, data trasparent */
#define MIP1535TRX_SYNC_MODE_SLOW            0x01 /*sync disabled, "slow" mode */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_1200   0x02 /*sync enabled, baud rate 1200 */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_2400   0x03 /*sync enabled, baud rate 2400 */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_4800   0x04 /*sync enabled, baud rate 4800 */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_9600   0x05 /*sync enabled, baud rate 9600 */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_19200  0x06 /*sync enabled, baud rate 19200 */
#define MIP1535TRX_SYNC_MODE_BAUDRATE_384000 0x07 /*sync enabled, baud rate 38400 */

#define MIP1535TRX_POWER_MODE_10dbm          0x00 /* set TRX output power to +10 dbm */
#define MIP1535TRX_POWER_MODE_7dbm           0x01 /* set TRX output power to +7 dbm */
#define MIP1535TRX_POWER_MODE_4dbm           0x02 /* set TRX output power to +4 dbm */
#define MIP1535TRX_POWER_MODE_1dbm           0x03 /* set TRX output power to +1 dbm */
#define MIP1535TRX_POWER_MODE__2dbm          0x04 /* set TRX output power to -2 dbm */
#define MIP1535TRX_POWER_MODE__5dbm          0x05 /* set TRX output power to -5 dbm */

#endif
