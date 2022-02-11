#ifndef MIPOT_32001269_32001534_H_
#define MIPOT_32001269_32001534_H_

#define MIP1534TRX_PROGRAMMING_TIMEOUT       50 /* ms */
#define MIP1534TRX_START_BYTE                0x18
#define MIP1534TRX_STANDART_CMD_LEN          0x05
#define MIP1534TRX_CHANNEL1_CMD_BYTE         0x50
#define MIP1534TRX_CHANNEL2_CMD_BYTE         0x51
#define MIP1534TRX_MODULATION_CMD_BYTE       0x52
#define MIP1534TRX_SYNC_CMD_BYTE             0x53
#define MIP1534TRX_POWER_CMD_BYTE            0x01
#define MIP1534TRX_RSSI_CMD_BYTE             0x55
#define MIP1534TRX_DEFAULTSETTING_CMD_BYTE   0x0A

#define MIP1534TRX_CHANNEL_FREQ_433_42       0x00 /* Set TRX CHX to 433.42 MHz */
#define MIP1534TRX_CHANNEL_FREQ_433_92       0x01 /* Set TRX CHX to 433.92 MHz */
#define MIP1534TRX_CHANNEL_FREQ_434_42       0x02 /* Set TRX CHX to 434.42 MHz */
#define MIP1534TRX_CHANNEL_FREQ_433_20       0x03 /* Set TRX CHX to 433.20 MHz */
#define MIP1534TRX_CHANNEL_FREQ_434_64       0x04 /* Set TRX CHX to 434.64 MHz */

#define MIP1534TRX_MODULATION_TYPE_OOK       0x00 /* trx modulation OOK */
#define MIP1534TRX_MODULATION_TYPE_2SFK      0x01 /* trx modulation 2FSK */

#define MIP1534TRX_SYNC_MODE_DISABLED        0x00 /*sync disabled, normal mode, data trasparent */
#define MIP1534TRX_SYNC_MODE_SLOW            0x01 /*sync disabled, "slow" mode */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_1200   0x02 /*sync enabled, baud rate 1200 */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_2400   0x03 /*sync enabled, baud rate 2400 */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_4800   0x04 /*sync enabled, baud rate 4800 */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_9600   0x05 /*sync enabled, baud rate 9600 */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_19200  0x06 /*sync enabled, baud rate 19200 */
#define MIP1534TRX_SYNC_MODE_BAUDRATE_384000 0x07 /*sync enabled, baud rate 38400 */

#define MIP1534TRX_POWER_MODE_10dbm          0x00 /* set TRX output power to +10 dbm */
#define MIP1534TRX_POWER_MODE_7dbm           0x01 /* set TRX output power to +7 dbm */
#define MIP1534TRX_POWER_MODE_4dbm           0x02 /* set TRX output power to +4 dbm */
#define MIP1534TRX_POWER_MODE_1dbm           0x03 /* set TRX output power to +1 dbm */
#define MIP1534TRX_POWER_MODE__2dbm          0x04 /* set TRX output power to -2 dbm */
#define MIP1534TRX_POWER_MODE__5dbm          0x05 /* set TRX output power to -5 dbm */

#endif
