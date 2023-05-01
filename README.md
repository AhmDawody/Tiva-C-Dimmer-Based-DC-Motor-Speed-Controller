## Project Overview
This project features a microcontroller-based dimmer circuit that controls the speed of a 220V-DC motor. The system includes a zero-crossing detector that generates a periodic pulse, helping to locate the starting point for triggering the Triac. The Triac is used to control the speed of the motor, resulting in precise and adjustable motor control.

## Project Features
1- The motor starting from stationary to avoid high starting current.<br>
2- Motor acceleration and deceleration are controlled through the microcontroller, resulting in smooth speed transitions.<br>
3- The system allows users to set a maximum speed for the motor, which is saved in non-volatile memory.<br>
4- An LCD screen displays the motor speed and maximum speed in real-time, providing valuable feedback on the performance of the system.

## Hardware Part
* ### Zero Crossing Detector and Triac Driver Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235466203-2dc88544-ca48-4964-93f0-8411b25b429f.jpg" width="400">
</div>
The circuit uses 5 and 3.3 V power supplies, you can use the microcontroller 5V, 3.3V pins, 220V AC is the motor supply voltage before rectification.
The right part of this schematic is zero crossing detector which generates a pulse of 5V each time the AC source crosses 0V, which helps to locate the starting point to fire the triac angle, the output is connected to PA6.<br>
The left part is triac driver with the load connected, this part is controlled by the microcontroller which is responsible of when to trigger the triac, after how many milliseconds of the starting point should I trigger the triac? the input of this part is connected to PA7.<br>

* ### LCD Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235471064-a9d7c2da-3b12-418a-b2cb-b95d2d2ccde1.jpg" width="200">
</div>

* 100nf capacitor is used to stablize the voltage on the variable resistor.
* LCD may has two additional pins: Anode and Kathode, which are connected to 5V and ground respectively.<br>

*  ### EEPROM Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235474384-e3f6f796-c4ab-4fe7-bf63-ccdeb1186790.jpg" width="200">
</div>

* EEPROM AT24C02.
* EEPROM speed is 100Kbps which is suitable with 4.6k Ohm resistors.
* EEPROM power is connected to 5V source.<br>

* ### ADC and Buttons Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235476778-a32e660b-26e0-4670-b68c-f9d5bc2654a1.jpg" width="200">
</div>

* PE0 is "Enter" button which is used to set maximum speed.
* PE2,3 are "Up/Down" buttons which are used to customize maximum speed.
* Internal pull down resistors is set.

## Software Part
* ### Microcontroller Initialization
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235486584-338fdc33-3c94-403d-ac46-275a9bfcd9e3.png" width="200">
</div><br>

The program is written using direct register access mode, and started with interrupts disabled, then **PLLInit()** is called to set system clock to 80Mhz to be suitable for ADC module, after that systick timer is initialized by calling **Sys_Init()** which is used to create time delays only, then LCD is initialized to operate on 4-bit mode by calling **LCD_Init()**, then ADC module initialized on PE1 by calling **ADCInit()**, then the buttons used to change/customize the motor maximum speed is initialized on port E using **BUTTONS_Init()**

