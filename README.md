## Project Overview
This project is a simple way to control the speed of 220V-DC Motor. a 50Hz-220V-AC zero crossing detector is used to generate a periodic pulse of 100Hz which helps to locate the starting point to fire the triac angle which is controlling the DC-motor.  
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

