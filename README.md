## Project Overview
This project is a simple way to control the speed of 220V-DC Motor. a 50Hz-220V-AC zero crossing detector is used to generate a periodic pulse of 100Hz which helps to locate the starting point to trigger triac  which is controlling the DC-motor.  
## Project Features
1- The motor starting from stationary to avoid high starting current.<br>
2- The changing in motor speed is done in accelerating/decelerating way.<br>
3- User can set the maximum speed which is reserved in non-volatile way.<br>
4- Motor Speed/Maximum speed is real time monitored on LCD.
## Hardware Part
Hardawre circuit consists of two parts:  
**First part :** "Zero crossing detector" which generates pulse of 5V every time 220V-50Hz-AC reaches zero volts.  
**Second part :** "Triac driver" which is enabled by microcontroller signal to control DC Motor.
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235359658-dbff0464-d32c-4146-bf52-9d7e73970af9.jpg" width="600">
</div>  
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235360290-bbaa80d2-3818-4908-8ff9-65cfdbc56bd2.jpg" width="400">
  <img src="https://user-images.githubusercontent.com/107086104/235360428-6668084e-23c7-4101-8afa-8235fcc184ca.jpg" width="400">
</div> <br>

**Additional components :** LCD 2*16, 3 push buttons, 2 100nf-25v capacitors, 24c02n EEPROM, 2 10k Ohms variable resistors and 2 4.6k , 220 Ohm resistors.<br>

## Software Part
The software part converts the variable resistor voltage by ADC module to a time delay before triggering the triac. this is done by initializing two kinds of interrupts: GPIO interrupt and one-shot mode timer interrupt.<br>
GPIO interrupt used to capture the signal coming from zero crossing detector then start a timer with specific reload value, once the reload value hits 0 the timer interrupt triggers the triac leading to starting the motor.<br>
The reload value is designed to start from the maximum reload value to start the motor from stationary to the desired speed.
