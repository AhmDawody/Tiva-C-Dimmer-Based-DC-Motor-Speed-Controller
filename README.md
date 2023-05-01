## Project Overview
This project features a microcontroller-based dimmer circuit that controls the speed of a 220V-DC motor. The system includes a zero-crossing detector that generates a periodic pulse, helping to locate the starting point for triggering the Triac. The Triac is used to control the speed of the motor, resulting in precise and adjustable motor control.
## Project Features
1- The motor starting from stationary to avoid high starting current.<br>
2- Motor acceleration and deceleration are controlled through the microcontroller, resulting in smooth speed transitions.<br>
3- The system allows users to set a maximum speed for the motor, which is saved in non-volatile memory.<br>
4- An LCD screen displays the motor speed and maximum speed in real-time, providing valuable feedback on the performance of the system.
## Hardware Part
Hardawre circuit consists of two parts:  
**First part :** "Zero crossing detector" which generates pulse of 5V every time 220V AC reaches zero volts.  
**Second part :** "Triac driver" which is enabled by microcontroller signal to control DC Motor.
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235359658-dbff0464-d32c-4146-bf52-9d7e73970af9.jpg" width="600">
</div>  
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235360290-bbaa80d2-3818-4908-8ff9-65cfdbc56bd2.jpg" width="400">
  <img src="https://user-images.githubusercontent.com/107086104/235360428-6668084e-23c7-4101-8afa-8235fcc184ca.jpg" width="400">
</div> <br>

**Additional components :** LCD 2*16, 3 push buttons, 2 100nf-25v capacitors, 24c02n EEPROM, 2 10k Ohms variable resistors and 2 4.6k Ohm , 220 Ohm resistors.<br>
* LCD is connected as 4-bit mode : DB0-3 to ground, DB4-7 to PC4-7, RS to PA2, E to PA3.
* ADC pin : PE1.
* I2C pins : PB2,3.
* 3 Push buttons is connected to PE0,2,3.
* 100nf capacitors is used with the variable resistors (between ground and movable pin) to stablize its voltage.
* 220 Ohm resistor is used between microcontroller output pin PA7 and triac firing pin to handle the current. 
* 2 4.6k Ohm resistors is used to connect **SDA** and **SCL** buses to 3.3V.<br>

## Software Part
The software part converts the variable resistor voltage by ADC module to a time delay before triggering the triac. this is done by initializing two kinds of interrupts: **GPIO interrupt** and **one-shot mode timer interrupt**.<br>
**GPIO interrupt** is used to capture the signal coming from zero crossing detector then start a timer with specific reload value, once the reload value hits 0 the **timer interrupt** triggers the triac leading to starting the motor.<br>
* The reload value is designed to start from the maximum reload value to start the motor from stationary to the desired speed.
* EEPROM is interfaced using I2C protocol with 100Kbps speed.
