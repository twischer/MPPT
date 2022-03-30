# MPPT
This is a Maximum Power Point Tracking library for PlatformIO supporting Arduino and WiringPi.

It is used by the following projects:
* https://github.com/twischer/MPPTCharger
* https://github.com/twischer/Tasmota/tree/mppt


## MPPT PWM Output
One of the following wiring can be used for this type of output.

### Via Dead time control of TL494
```
                   _______
                  | TL494 |
              +---+REF    |
            C1|   |       |
             10µF |       |
       R2     |   |       |
GPIO--<1k2>---+---+DTC    |
            R1|   |       |
             10k  |       |
              |   |       |
              +---+GND    |
                  |_______|
```
C1 and R1 are already present on many voltage converters using a TL494.
Therefore only the additional R2 is required for this solution.

### Via Feedback of XL4016
```
                           ________
       R3    D1           | XL4016 |
GPIO--<240>--|>----+---+--+FB      |
                 R1| R2|  |        |
                  2k2 430 |        |
                   |   |  |        |
                   +---+--+GND     |
                          |________|
```
R1 and R2 are already present on many step down converters using a XL4016.
Therefore only the additional R3 and D1 are required for this solution.
Keep in mind that a reverse current will flow from batteries throw XL4016 back to the
solar panel in the night. Therefore disconnect it or add an additional diode.


## MPPT Sigma Delta Output
This type of output provides a higher output frequency.
Therefore a smaller RC filter is required.
The same wiring as for the PWM output can be used.

