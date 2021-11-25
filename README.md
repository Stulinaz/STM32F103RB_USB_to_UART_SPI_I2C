# STM32F103RB__USB_to_UART-SPI-I2C
Firmware and Kicad project for my USBtoUART/SPI/I2C converter.

[PROJECT IN DEVELOPMENT]

Based on the popular microcontroller STM32F103RBT6 and a custom PCB board.
This project is an embedded software/hardawe solutions that allows you to transfer data from USB to all the main digital interfaces - UART, I2C and SPI.
This board will help you to analyze and debug all the main digital and analog IC’s as well as lot of IOT and RF demoboard, saving you the time to develop a custom firmware
, or buying expensive data converters.

INFORMATION ON FIRMWARE DEVELOPMENT:

DEVELIPMENT IDE: STM32CubeIDE Version: 1.6.0

TOOLCHAIN: GNU Tools for STM32 (9-2020-q2-update)

TOP OF PCB:
![Test Image 1](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG4_TOP.JPG)

BOTTOM OF PCB:
![Test Image 2](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG1_BOT.JPG)

FEATURES:

SWD INTERFACE

![Test Image 3](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG3_SWD.JPG)

MINI USB WITH ESD PROTECTION:

![Test Image 4](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG5_USB.JPG)

POWER MOSFET

![Test Image 5](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG2_MOS.JPG)



CRITICAL COMPONENTS:

-component [USB1]: PART NUMBER:[0675031020] - DESCRIPTION:[CONN RCPT USB2.0 MINI SMD R/A] - DIGIKEY:[WM5461CT-ND]

-component [ESD1]: PART NUMBER:[USBLC6-2SC6] - DESCRIPTION[TVS DIODE 5.25V 17V STO23-6] - DIGIKEY:[732-1077-1-ND]

-component [ESD2]: PART NUMBER:[ESDA6V1BC6] - DESCRIPTION[TVS DIODE 5V SOT23-6] - DIGIKEY:[497-6635-1-ND]

-component [U1] : PART NUMBER:[MIC39100-3.3WS] - DESCRIPTION[Linear Voltage Regulator IC Positive Fixed 1 Output 1A SOT-223-3] - DIGIKEY:[576-1172-ND]

-component [U2]  : PART NUMBER:[STM32F103RBT6] - DESCRIPTION[ic mcu 32 bit 128kb flash 64LQFP] - DIGIKEY:[497-6066-ND]

-component [Y2]  : PART NUMBER:[ABM8W-16.0000MHZ-4-B1U-T3] - DESCRIPTION[CRYSTAL 16.0000MHZ 4PF SMD] - DIGIKEY:[535-14020-1-ND]

-component [USB2]  : PART NUMBER:[LG R974-LP-1] - DESCRIPTION[LED GREEN DIFFUSED 0805 SMD] - DIGIKEY:[475-1410-1-ND]

-component [U3]  : PART NUMBER:[MIC4427ZM] - DESCRIPTION[IC GATE DRVR LOW-SIDE 8SOIC] - DIGIKEY:[576-3513-5-ND]

MY DEBUG SETUP:

![Test Image 6](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/PHOTOS/IMG6_BOARD.JPG)

