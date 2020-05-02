# Arduino-Ghost-Trap

This is an example code to make a Ghostbusters Ghost Trap Circuit.


Before using this code :

Ensure you have libraries listed below :

-__DFPlayer_Mini_Mp3__

-__softwareSerial__


______________________________________________________________________

__BY USING THIS PROJECT, YOU ACCEPT THAT PROTONGAMER IS NOT RESPONSIBLE OF ANY DAMAGE AND YOU
USING IT AT YOUR OWN RISK ! THIS OPEN SOURCE PROJECT IS A FREE USE AND YOU ACCEPT THAT YOU WILL NOT 
MAKE ANY PROFITS !__

______________________________________________________________________


## Compatible Arduino boards


-__Arduino Uno__
-__Arduino Nano__
-__Arduino Pro Mini__
-__Arduino Mega 2560__

-__Other Board Based on ATMega2560 / ATMega328 / ATMega168__

____________________________________________________________________________________________________

## Parameters

You have parameters in the header file __parameters.h__ that you can change values to configure __GHOST_TRAP_SIMPLE.ino__

| Parameter | Range values | Descritpion |
| --------- | ------------ | ----------- |
| USE_SOFTWARE_SERIAL | Comment / Uncomment | Control Mp3 Dfrobot module by using software serial or harware serial(by using harware serial, default debug will be unused) |
| VOLUME | 0-30 | Set Mp3 module volume |
| FLICKER_MAX | 0-255 | Set max bright random value for flickering effect |
| FLICKER_MIN | 0-255 | Set min bright random value for flickering effect |
| SMOKE_TIME | 0-65535 | Set time that the smoke kit is active in milliseconds(1000 ms = 1 s) |
| BLINK_TIME_CAPT | 0-65535 | Set blink time for the red led when captured ghost sequence is done in milliseconds |
| BLINK_TIME_SLEEP | 0-65535 | Set blink time for the red led when ghost trap is awaiting user button press in milliseconds |
| OFFSET | (-32768)-(+32767) | Set an offset value when microcontroller's ADC read sound to detect sparks sounds for blue flicker effect |
| MAX_SPARK | 15-50(advice) | If spark sound detected, set an offset compare value read by ADC for flashing effect(Lower value -> More Bright , see analogWrite lines 340 and 343) |
| MIN_DEGREE_1 | 0-180 | Set min value of servo motor 1 in degree |
| MAX_DEGREE_1 | 0-180 | Set max value of servo motor 1 in degree |
| MIN_DEGREE_2 | 0-180 | Set min value of servo motor 2 in degree |
| MAX_DEGREE_2 | 0-180 | Set max value of servo motor 2 in degree |

____________________________________________________________________________________________________

## Bargraph

The Ghost Trap's bargraph is made by 2x5 segments leds. Each commons(of 5 segments leds) are swapped(see Timer 1 interrupt)
