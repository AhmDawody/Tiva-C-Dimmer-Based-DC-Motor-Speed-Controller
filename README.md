## Project Overview
This project is a simple way to control the speed of 220V-DC Motor. a 50Hz-220V-AC zero crossing detector is used to generate a periodic pulse of 100Hz which helps to locate the starting point to fire the triac angle which is controlling the DC-motor.  
## Hardware Part
Hardawre circuit consists of two parts:  
First part is "zero crossing detector" which generates pulse of 5V every time 220V-50Hz-AC reaches zero volts.  
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235358185-ee56f6a0-80a8-46ac-954d-a3c02eb1c239.png" width="600">
</div>  

Second part is "Triac driver" which is enabled by micro controller signal which controls DC Motor.
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235358760-59b97d05-6a52-4ee7-90d2-fc81628af656.png" width="600">
</div> 
