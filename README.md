# PineapplePad
TM4C based handheld gaming device

## Pinout

- Port A
	- A0: UART Rx
	- A1: UART Tx
	- A2: Sclk
	- A3: TFT_CS
	- A4: MISO from SDC to microcontroller
	- A5: MOSI from microcontroller to TFT
	- A6: TFT data/command
	- A7: TFT reset
- Port B
- Port D
	- D2: ADC in (A5)
	- D3:
	- D6:
	- D7: SDC_CS
- Port E
	- E5: Audio Out
- Port F
	- F0: Onboard Button 2
	- F1: RED_LED
	- F2: BLUE_LED
	- F3: GREEN_LED
	- F4: Onboard Button 1

## Software todo

- HAL
    - [x] SPI to LCD
    - [x] SPI from SD card
    - [] SPI from accelerometer
    - [x] ADC from slide pot
    - [] Button GPIO interrupts
- Game
    - [x] Game logic
    - [] Highscore logic (eeprom or SD card)
- Render
    - [x] Draw splash
    - [x] Draw frame
    - [x] Draw score
- Sound
    - [x] Filesystem reading
    - [x] Audio file streaming

## Priority
- IO: 5
- DiskIO/Timer5: 2
- Audio/Systick: 0
- Graphics/Timer1: 7

## Hardware todo

- [] PCB with double 2x10 male headers connects to screen. 1x8 to DAC/AMP, 1x8 to accelerometer, pads for buttons
- [] PCB for DAC and AMP with leads to speaker, slide pot for volume, switch for 3.5mm
- [] 3d print housing for battery + speaker + 2 pcb's with screws to mount them

## Resources

- [author's explanation](https://web.archive.org/web/20060615142050/http://www.flecko.net:80/cubefield.html)
- [3d projection wikipedia](https://en.wikipedia.org/wiki/3D_projection)
- [3d projection tutorial](https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points)
