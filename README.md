<!-- For .md file development refers to https://docs.github.com/en -->
# Stm32 Multiprotocol

> [!IMPORTANT]  
> Firmware rework scheduled by end of April 2025.

### Firmware and Kicad project for my USBtoUART/SPI/I2C converter.

Based on the popular microcontroller ***STM32F103RBT6*** and a custom PCB board.
This project is an embedded software/hardawe solutions that allows you to transfer data from USB to all the main digital interfaces - UART, I2C and SPI.
This board will help you to analyze and debug all the main digital and analog IC’s as well as lot of IOT and RF demoboard, saving you the time to develop a custom firmware
, or buying expensive data converters.

## Board Overview

![img004](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img004.png)

## Top Side of the board

![img000](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img000.png)

## Bottom Side of The board

![img001](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img001.png)

## Features 

- SWD Interface.
- Mini USB Connector with ESD protection.
- Power mosfet.
- On board 64k (24C64) eeprom for data storage.

![img002](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img002.png)

![img003](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img003.png)

## Bill of Materials

| Component  | Description                      | Part Number              | Digy           |
|:---------  |:----------                       |:-------                  |:-------        |        
| USB1       | CONN RCPT USB2.0 MINI SMD R/A    | 0675031020               | [WM5461CT-ND](https://www.digikey.it/it/products/detail/molex/0675031020/1624424?s=N4IgTCBcDaIOoFkCsAWAbARgMIBUC0AcgCIgC6AvkA)                      |
| ESD1       | TVS DIODE 5.25V 17V STO23-6      | USBLC6-2SC6              | [497-5235-6-ND](https://www.digikey.it/it/products/detail/stmicroelectronics/USBLC6-2SC6/1040559)                                                   |
| ESD2       | TVS DIODE 5V SOT23-6]            | ESDA6V1BC6               | [497-6635-1-ND](https://www.digikey.it/it/products/detail/stmicroelectronics/ESDA6V1BC6/1037985?s=N4IgTCBcDaICwE4DsBaAbGgzAVhQRhQDkAREAXQF8g)       |
| U1         | Linear Voltage Regulator IC      | IC REG 3.3V 1A           | [576-1172-ND](https://www.digikey.it/it/products/detail/microchip-technology/MIC39100-3-3WS/771641?s=N4IgTCBcDaIKwHYBsBaAjGhYUDkAiIAugL5A)          |
| U2         | ic mcu 32 bit 128kb flash 64LQFP | STM32F103RBT6            | [497-6066-ND](https://www.digikey.it/it/products/detail/stmicroelectronics/STM32F103RBT6/1646341?s=N4IgTCBcDaICwE4DsBaAbABjWlA5AIiALoC%2BQA)        |
| Y2         | CRYSTAL 16.0000MHZ 4PF SMD       | ABM8W-16.0000MHZ-4-B1U-T3| [535-14020-1-ND](https://www.digikey.it/it/products/detail/abracon-llc/ABM8W-16-0000MHZ-4-B1U-T3/7172791?s=N4IgTCBcDaIKwGY4FoCMAWADGTbkDkAREAXQF8g) |
| U3         | IC GATE DRVR LOW-SIDE 8SOIC      | MIC4427ZM                | [576-3513-5-ND](https://www.digikey.it/it/products/detail/microchip-technology/MIC4427ZM/1029913?s=N4IgTCBcDaIKwHYBsBaAzHAjGlcUDkAREAXQF8g)         |
| U4         | IC GATE DRVR LOW-SIDE 8SOIC      | M24C64                   | [497-14427-1-ND](https://www.digikey.it/it/products/detail/stmicroelectronics/M24C64-DRMN3TP-K/4729231?s=N4IgTCBcDaICwE4DsBaAjHOZVpQOQBEQBdAXyA)    |     

## Software needed for development

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) **1.17.0**
- [KICAD](https://www.kicad.org/) **8.0.5,release build - 64 bit**
- [GIT](https://git-scm.com/) **2.46.1.windows.1**

## Tools needed for development

Programmer [STLINK](https://www.st.com/en/development-tools/st-link-v2.html)

## Closeup

![img005](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img005.png)

![img006](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img006.png)

![img007](https://github.com/Stulinaz/STM32F103RB_USB_to_UART_SPI_I2C/blob/master/img/img007.png)

## Electrical Characteristics

| **Parameter**         | **Min** | **Typ** | **Max** | **Unit** | **Condition/Notes**   |
|-----------------------|---------|---------|---------|----------|-----------------------|
| Input Voltage         | 4.5     | 5.0     | 5.5     | V        |                       |
| Current Consumption   |         | 13      | 20      | mA       |                       |

## License

| Component                                    | Copyright                          | License                                          |
|:---------                                    |:----------                         |:-------                                          |
| CMSIS                                        | ARM Limited                        | Apache-2.0                                       |
| CMSIS Device                                 | ARM Limited - STMicroelectronics   | Apache-2.0                                       |
| STM32WL HAL                                  | STMicroelectronics                 | BSD-3-Clause                                     |
| FreeRTOS kernel                              | Amazon.com, Inc. or its affiliates | MIT                                              |
| Stm32 Multiprotocol                          | N/A                                | BSD-3-Clause                                     |

## Contact

***enrico.stulin@gmail.com***

[LinkedIn](https://www.linkedin.com/in/enrico-stulin-14090b146/)

## Important Information

> [!CAUTION]
> Manual soldering of SMD components requires high level of manual skills.
> Project not suited for beginners in firmware development.
> A good understanding of C language is a must.

> [!CAUTION]
> This project is provided "AS IS" with no warranties. 