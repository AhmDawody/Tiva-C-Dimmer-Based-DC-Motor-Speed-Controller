/*-----------------------------------------------
 * File:        main.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Main file.
 *---------------------------------------------*/

#include "global.h"

int main(void){
    __asm(" CPSID  I ");                        // Disable Interrupts(Global)
    PLLInit();                                  // System clock 80MHz
    Sys_Init();                                 // Systick timer, for time delays
    LCD_Init();                                 // LCD display
    ADCInit();                                  // ADC module
    BUTTONS_Init();                             // Push buttons used to change max speed
    PortA_Init();                               // PortA used in capturing signals
    I2C_Init();                                 // I2C protocol used to communicate with EEPROM
    Update_max();                               // Update maximum speed to be used
    Timer2_Init();                              // Timer 2 for firing the angle
    LCD_Cmd(LCD_CLEAR);                         // Clear LCD at the beginning
    Sys_Wait10ms(50);                           // Wait 0.5 s
    __asm(" CPSIE  I ");                        // Enable Interrupts(Global)
    while(1){
        update_screen();                        // Update program screen
        BUTTONS_Check();                        // Check for button if clicked
    }
}
