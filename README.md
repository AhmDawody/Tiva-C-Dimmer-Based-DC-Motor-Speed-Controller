# Project Overview
This project features a microcontroller-based dimmer circuit that controls the speed of a 220V-DC motor. The system includes a zero-crossing detector that generates a periodic pulse, helping to locate the starting point for triggering the Triac. The Triac is used to control the speed of the motor, resulting in precise and adjustable motor control.

# Project Features
1- The motor starting from stationary to avoid high starting current.<br>
2- Motor acceleration and deceleration are controlled through the microcontroller, resulting in smooth speed transitions.<br>
3- The system allows users to set a maximum speed for the motor, which is saved in non-volatile memory.<br>
4- An LCD screen displays the motor speed and maximum speed in real-time, providing valuable feedback on the performance of the system.

# Hardware Part
* ## Zero Crossing Detector and Triac Driver Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235466203-2dc88544-ca48-4964-93f0-8411b25b429f.jpg" width="400">
</div>
The circuit uses 5 and 3.3 V power supplies, you can use the microcontroller 5V, 3.3V pins, 220V AC is the motor supply voltage before rectification.
The right part of this schematic is zero crossing detector which generates a pulse of 5V each time the AC source crosses 0V, which helps to locate the starting point to fire the triac angle, the output is connected to PA6.<br>
The left part is triac driver with the load connected, this part is controlled by the microcontroller which is responsible of when to trigger the triac, after how many milliseconds of the starting point should I trigger the triac? the input of this part is connected to PA7.<br>

* ## LCD Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235471064-a9d7c2da-3b12-418a-b2cb-b95d2d2ccde1.jpg" width="200">
</div>

* 100nf capacitor is used to stablize the voltage on the variable resistor.
* LCD may has two additional pins: Anode and Kathode, which are connected to 5V and ground respectively.<br>

*  ## EEPROM Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235474384-e3f6f796-c4ab-4fe7-bf63-ccdeb1186790.jpg" width="200">
</div>

* EEPROM AT24C02.
* EEPROM speed is 100Kbps which is suitable with 4.6k Ohm resistors.
* EEPROM power is connected to 5V source.<br>

* ## ADC and Buttons Schematic
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235476778-a32e660b-26e0-4670-b68c-f9d5bc2654a1.jpg" width="200">
</div>

* PE0 is "Enter" button which is used to set maximum speed.
* PE2,3 are "Up/Down" buttons which are used to customize maximum speed.
* Internal pull down resistors is set.

# Software Part
* ## Microcontroller Initialization
```
int main(void){
    __asm(" CPSID  I "); 
    PLLInit();
    Sys_Init();  
    LCD_Init();  
    ADCInit();                 
    BUTTONS_Init();
    PortA_Init();                
    I2C_Init();                    
    Update_max();                      
    Timer2_Init();    
    LCD_Cmd(LCD_CLEAR);
    Sys_Wait10ms(50); 
    __asm(" CPSIE  I "); 
```
The program is written using direct register access mode on TM4C123GH6PM microcontroller.<br> 
Initialization started with interrupts disabled, then **PLLInit()** is called to set system clock to 80MHz to be suitable for ADC module, after that systick timer is initialized by calling **Sys_Init()** which is used to create time delays only, then LCD operated on 4-bit mode by calling **LCD_Init()**, then ADC module on PE1 by calling **ADCInit()**, then the buttons used to change/customize the motor maximum speed on portE using **BUTTONS_Init()**, then portA which is responsible of capturing zero crossing signals and starting the timer2 by calling **PortA_Init()**, then I2C module which is used to communicate with EEPROM by calling **I2C_Init()**, then **Update_max()** is called which is used to recieve the maximum value stored in EEPROM to start the program with this value, then timer 2 which is used to triggering triac by calling **Timer2_Init()**, finally clearing LCD and enabling interrupts to start the program.<br>

* ## Triac Firing and Motor Speed Control
Triac is controlled using two types of interrupts : **GPIO Interrupts** on portA and **Timer2 one-shot mode Interrupts**.<br>
GPIO interrupts is used to capture the signal of zero crossing detector, every time the AC voltage crosses 0V an interrupt happen.
```
// PortA ISR
// Changing motor speed depending on ADC value in accelerating/decelerating way
// ----------------------------------------------------------------------------
void GPIOPortA_Handler(void){
    sample = convert();                                 // Sample from ADC
    if(reload>sample){
        reload -=280;                                   // -3.5us (Acceleration)
    }
    else if(reload<sample){
        reload +=280;                                   // +3.5us (Deceleration)
    }
    Timer2A_Start(reload);                              // Start timer2
    GPIO_PORTA_ICR_R |= (1<<6);                         // Clear flag
}
```
* PortA ISR job is starting timer2 with specific value depending on the value of variable resistor, starting from maximum reload value which is the most delay time before triggering triac to start motor from stationary and increase its speed gradually at specific rate.<br>
The software comparing the sample coming from ADC with the value to reload the timer with (the same way PI controller operate with).
```
// Timer2A ISR
// -----------
void Timer2A_Handler(void){
    Trigger();
    TIMER2_ICR_R = 0x00000001;                              // Acknowledge
}
```
* Timer2 ISR job is to trigger triac when its reload value goes from 1 to 0.<br>

* ## LCD Display and User Input Handling

* The program display just has two screens, one to display motor speed and maximum speed and the other one to change the maximum speed, by calling the function **update_screen()** on the main program loop.<br>
<div>
  <img src="https://user-images.githubusercontent.com/107086104/235498633-51da4c24-c1b9-46e8-9dd4-92cf7344c2b2.jpg" width="400">
  <img src="https://user-images.githubusercontent.com/107086104/235498687-4d951813-aec6-4a29-9709-eeade77ad925.jpg" width="400">
</div>
* User Input handling is done by checking if user clicked buttons every time function **BUTTONS_Check()** is called by polling over the main program loop.

```
    if(screen == 1){                                            // Main screen
        if(enter && prev_enter == 0){
            LCD_Cmd(LCD_CLEAR);
            screen =2;
        }
    }
    else if(screen == 2){                                       // Second screen
        if(up){
            inc_max();
        }
        else if(down){
            dec_max();
        }
        else if(enter && prev_enter == 0){
            // Send max value to be saved on startup
            max = get_max();
            I2C_Send(max);
            // Clear screen on go to main screen
            LCD_Cmd(LCD_CLEAR);
            screen = 1;
        }
    }
    prev_enter =enter;
}
```

* Enter button is used to switch between main and secondary screens, but it has another function if used in the secondary screen which is saving the maximum value in the EEPROM.
