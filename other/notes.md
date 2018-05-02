## Software todo

- HAL
    - [x] SPI to LCD
    - [x] SPI from SD card
    - [] SPI from accelerometer
    - [x] ADC from slide
    - [] Button GPIO interrupts
- Game
    - [] Menu logic
    - [x] Game logic
    - [] Highscore logic
- Render
    - [] Draw menu
    - [x] Draw frame
    - [] Draw score
- Sound
    - [x] Filesystem reading
    - [] File loading management

## Hardware todo

- [] PCB with double 2x10 male headers connects to screen. 1x8 to DAC/AMP, 1x8 to accelerometer, pads for buttons
- [] PCB for DAC and AMP with leads to speaker, slide pot for volume, switch for 3.5mm
- [] 3d print housing for battery + speaker + 2 pcb's with screws to mount them

## Resources

- [author's explanation](https://web.archive.org/web/20060615142050/http://www.flecko.net:80/cubefield.html)
- [3d projection wikipedia](https://en.wikipedia.org/wiki/3D_projection)
- [3d projection tutorial](https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points)

## Priority
- IO: 5
- DiskIO/Timer5: 2
- Audio/Systick: 0
- Graphics/Timer1: 8