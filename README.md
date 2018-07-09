# PineapplePad
> Could have been better...

![menu](https://photos.app.goo.gl/qK8PZpjbENy8hcSy6)
![death](https://photos.app.goo.gl/eTu5CZ2k6BfHNGmo9)

## Pinout

- Port A
    - A0: UART Rx
    - A1: UART Tx
    - A2: Sclk
    - A3: TFT_CS
    - A4: MISO
    - A5: MOSI
    - A6: TFT data
    - A7: TFT reset
- Port D
    - D2: Slide Pot
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
    - [ ] SPI from accelerometer
    - [x] ADC from slide pot
    - [ ] Button GPIO interrupts
- Game
    - [x] Game logic
    - [ ] Highscore logic (eeprom or SD card)
- Render
    - [x] Draw splash
    - [x] Draw frame
    - [x] Draw score
- Sound
    - [x] Filesystem reading
    - [x] Audio file streaming
    - [x] Midi conversion and playing
    - [x] Multiple instruments

## Hardware todo

- [ ] PCB with double 2x10 male headers connects to screen. 1x8 to DAC/AMP, 1x8 to accelerometer, pads for buttons
- [ ] PCB for DAC and AMP with leads to speaker, slide pot for volume, switch for 3.5mm
- [ ] 3d print housing for battery + speaker + 2 pcb's with screws to mount them

## Resources

- [Inspiration](https://web.archive.org/web/20060615142050/http://www.flecko.net:80/cubefield.html)
- [3d projection wikipedia](https://en.wikipedia.org/wiki/3D_projection)
- [3d projection tutorial](https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points)
- [Midi conversion](https://github.com/Tonejs/MidiConvert)
