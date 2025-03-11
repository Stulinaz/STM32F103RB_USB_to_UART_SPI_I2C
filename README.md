
<!-- For .md file development refers to https://docs.github.com/en -->
# Stm32 Multiprotocol

> [!IMPORTANT]  
> Firmware rework scheduled by end of april.

### Firmware and Kicad project for my USBtoUART/SPI/I2C converter.

Based on the popular microcontroller ***STM32F103RBT6*** and a custom PCB board.
This project is an embedded software/hardawe solutions that allows you to transfer data from USB to all the main digital interfaces - UART, I2C and SPI.
This board will help you to analyze and debug all the main digital and analog IC’s as well as lot of IOT and RF demoboard, saving you the time to develop a custom firmware
, or buying expensive data converters.

## Top Side of the board

![Test Image 1](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG4_TOP.JPG)

## Bottom Side of The board

![Test Image 2](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG1_BOT.JPG)

## Features 

### SWD Interface

![Test Image 3](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG3_SWD.JPG)

###  Mini USB Connector with ESD protection:

![Test Image 4](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG5_USB.JPG)

###  Power mosfet

![Test Image 5](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG2_MOS.JPG)

### On board 64k (24C64) eeprom for data storage:

![Test Image 8](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG8_EEPROM.JPG)


## Bill of Materials

| Component  | Description                      | Part Number              | Digy           |
|:---------  |:----------                       |:-------                  |:-------        |        
| USB1       | CONN RCPT USB2.0 MINI SMD R/A    | 0675031020               | WM5461CT-ND    |
| ESD1       | TVS DIODE 5.25V 17V STO23-6      | USBLC6-2SC6              | 732-1077-1-ND  |
| ESD2       | TVS DIODE 5V SOT23-6]            | ESDA6V1BC6               | 497-6635-1-ND  |
| U1         | Linear Voltage Regulator IC      | BSD-3-Clause             | 576-1172-ND    |
| U2         | ic mcu 32 bit 128kb flash 64LQFP | STM32F103RBT6            | 497-6066-ND    |
| Y2         | CRYSTAL 16.0000MHZ 4PF SMD       | ABM8W-16.0000MHZ-4-B1U-T3| 535-14020-1-ND |
| U3         | IC GATE DRVR LOW-SIDE 8SOIC      | MIC4427ZM                | 576-3513-5-ND  |
| U4         | IC GATE DRVR LOW-SIDE 8SOIC      | M24C64                   | 497-14427-1-ND |     

## Software needed for development

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) **1.17.0**
- [KICAD](https://www.kicad.org/) **(5.1.5)-2,release build - 64 bit**
- [GIT](https://git-scm.com/) **2.46.1.windows.1**
- [Tera Term](https://teratermproject.github.io/index-en.html) **5.3 x86**

## Tools needed for development

Programmer [STLINK](https://www.st.com/en/development-tools/st-link-v2.html).

## My Debug setup

### hardware revision 1

![Test Image 6](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG6_BOARD.JPG)

### hardware revision 2

![Test Image 7](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/IMG7_BOARD_REV2.JPG)

## License

| Component                                    | Copyright                          | License                                          |
|:---------                                    |:----------                         |:-------                                          |
| CMSIS                                        | ARM Limited                        | Apache-2.0                                       |
| CMSIS Device                                 | ARM Limited - STMicroelectronics   | Apache-2.0                                       |
| STM32WL HAL                                  | STMicroelectronics                 | BSD-3-Clause                                     |
| FreeRTOS kernel                              | Amazon.com, Inc. or its affiliates | MIT                                              |
| USB_to_UART-SPI-I2C                          |                                    | BSD-3-Clause                                     |

## Contact

***enrico.stulin@gmail.com***

> [!CAUTION]
> Manual soldering of SMD components requires high level of manual skills.
> Project not suited for beginners in firmware development.
> A good understanding of C language is a must.