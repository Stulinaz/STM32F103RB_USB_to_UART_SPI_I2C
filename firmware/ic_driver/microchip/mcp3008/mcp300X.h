#ifndef MICROCHIP_MCP3008_MCP300X_H_
#define MICROCHIP_MCP3008_MCP300X_H_

#define MCP3004 0
#define MCP3008 1
#define IC_TYPE MCP3008

#define START_BIT 0x10
#if (IC_TYPE == MCP3004)
#define CHANNEL0_MODE_SINGLE_ENDED 0x08 /* Single ended / Channel selection: CH0 */
#define CHANNEL1_MODE_SINGLE_ENDED 0x09 /* Single ended / Channel selection: CH1 */
#define CHANNEL2_MODE_SINGLE_ENDED 0x0A /* Single ended / Channel selection: CH2 */
#define CHANNEL3_MODE_SINGLE_ENDED 0x0B /* Single ended / Channel selection: CH3 */
#define CH0_CH1_MODE1_DIFFERENTIAL 0x00 /* Differential / CH0=IN+ / CH1=IN- */
#define CH0_CH1_MODE2_DIFFERENTIAL 0x01 /* Differential / CH0=IN- / CH1=IN+ */
#define CH2_CH3_MODE1_DIFFERENTIAL 0x02 /* Differential / CH2=IN+ / CH3=IN- */
#define CH2_CH3_MODE2_DIFFERENTIAL 0x03 /* Differential / CH2=IN- / CH3=IN+ */
#elif (IC_TYPE == MCP3008)
#define CHANNEL0_MODE_SINGLE_ENDED 0x08 /* Single ended / Channel selection: CH0 */
#define CHANNEL1_MODE_SINGLE_ENDED 0x09 /* Single ended / Channel selection: CH1 */
#define CHANNEL2_MODE_SINGLE_ENDED 0x0A /* Single ended / Channel selection: CH2 */
#define CHANNEL3_MODE_SINGLE_ENDED 0x0B /* Single ended / Channel selection: CH3 */
#define CHANNEL4_MODE_SINGLE_ENDED 0x0C /* Single ended / Channel selection: CH4 */
#define CHANNEL5_MODE_SINGLE_ENDED 0x0D /* Single ended / Channel selection: CH5 */
#define CHANNEL6_MODE_SINGLE_ENDED 0x0E /* Single ended / Channel selection: CH6 */
#define CHANNEL7_MODE_SINGLE_ENDED 0x0F /* Single ended / Channel selection: CH7 */
#define CH0_CH1_MODE1_DIFFERENTIAL 0x00 /* Differential / CH0=IN+ / CH1=IN- */
#define CH0_CH1_MODE2_DIFFERENTIAL 0x01 /* Differential / CH0=IN- / CH1=IN+ */
#define CH2_CH3_MODE1_DIFFERENTIAL 0x02 /* Differential / CH2=IN+ / CH3=IN- */
#define CH2_CH3_MODE2_DIFFERENTIAL 0x03 /* Differential / CH2=IN- / CH3=IN+ */
#define CH4_CH5_MODE1_DIFFERENTIAL 0x04 /* Differential / CH4=IN+ / CH5=IN- */
#define CH4_CH5_MODE2_DIFFERENTIAL 0x05 /* Differential / CH4=IN- / CH5=IN+ */
#define CH6_CH7_MODE1_DIFFERENTIAL 0x06 /* Differential / CH6=IN+ / CH7=IN- */
#define CH6_CH7_MODE2_DIFFERENTIAL 0x07 /* Differential / CH6=IN- / CH7=IN+ */
#endif

#endif
