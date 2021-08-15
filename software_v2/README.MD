# Software for controlling the split flap display

## Features

### Drivers 

- [x] Create motor driver
- [x] Create hall effect sensor driver
- [x] Create UART driver
- [x] Create WiFi driver

### Other

- [x] Fetch current bitcoin value and display using the split flap
- [ ] Create a webserver to allow for control over the split flap from another device
- [ ] Option to display the time

## Hardware

The files made by ST and ARM are hardware-specific, and should be checked and modified before running this yourself.

The driver files should work with most, if not all, STM32 devices (I have not verified this, though I believe they are all relatively similar in terms of how they function, though they still may require some modification). You will of course have to ensure all the pins and GPIOs are set correctly.

## Setup

Ensure you are using a correct `.ld` file for your target hardware, and have also set the correct hardware options in `Makefile`. Then simply run `make` in this directory, which will create a file named `main.elf`. You should then flash this file to your hardware.

### STM32F76xx and STM32F77xx Useful Links

[https://www.st.com/resource/en/reference_manual/dm00224583-stm32f76xxx-and-stm32f77xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf](https://www.st.com/resource/en/reference_manual/dm00224583-stm32f76xxx-and-stm32f77xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

#### STM32F767ZI Useful Links

[https://www.st.com/resource/en/datasheet/stm32f767zi.pdf](https://www.st.com/resource/en/datasheet/stm32f767zi.pdf)