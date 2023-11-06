# receiver_gauge

## Features
- A large, 320x480 TFT display using an ILI9488 LCD controller
- Touchscreen support
- Multiple display modes:
    - Display of critical engine information via realtime dials/meters
    - Realtime graphing of engine parameters for tuning
- A serial interface to a Speeduino ECU
- An SD datalogger

## Mode of operation
### Serial interface to Speeduino
The dash will poll data from Speeduino.
Each polling will pull realtime data from the Speeduino via the Secondary Serial IO interface (see https://wiki.speeduino.com/en/Secondary_Serial_IO_interface). For the legacy realtime data set this is 75 bytes of data returned after a one byte command (5.2ms of time at 115200bps). For the complete realtime data set this is 120 bytes of data returned after a 1 byte command (8.3ms of time at 115200bps).

The data protocol used is described at https://wiki.speeduino.com/en/reference/Interface_Protocol.

There is an option to use USB which looks interesting.

### Display
The display will have a number of modes:
- A "Gauge" mode, where realtime data is displayed on a number of dials/gauges
- A "Graph" mode, where realtime parameters are plotted on a graph vs. time
- A "Debug" mode, where realtime information is displayed on the screen via text

#### Touchscreen support

### SD logging

## Hardware Setup
### Screen
The screen used is a 3.5" 320x480 ILI9488 LCD of the "MCUFRIEND" Arduino shield type. It has a resistive touchscreen controller. The pinout of the shield is as follows:
(picture of back of screen)
(list of pins and definitions)

### Touchscreen
The touchscreen is a resistive-type and is interfaced with by pins XYZ. It can detect single touch events.
NOTE: Abstracting the screen controller from the touch events would be helpful here to enable future development with alternative touchscreens (I2C interfaces, capacitive touch, etc.).
### SD Interface
The screen module includes an SPI SD card interface.

### MCU
The microcontroller used is an ESP32-S3. It is connected to the display as follows:
(list of pins and connections)
The use of the ESP32-S3-WROOM-1 MON8R8 provides 8MB of SPI flash and 8MB of OSPI PSRAM. This is helpful for additional graphic resources.
The FREENOVE ESP32-S3-WROOM CAM Board was used for this project. It includes a USB port, a USB OTG port, and a 24-pin FPC connector that may be repurposed in the future to interface with the LCD (further investigation required). The pins for the FPC are shared with GPIO pins.

## Software Setup
- Display: TFT_eSPI
    - The excellent [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library by [Bodmer](https://github.com/Bodmer) is used to drive the display.
- Touchscreen:
- SD Card:
