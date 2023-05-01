/*-----------------------------------------------
 * File:        buttons.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Source file of program buttons.
 *---------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "lcd/lcd.h"
#include "screen.h"
#include "adc/adc.h"
#include "I2C/I2C.h"
#include "delay/delay.h"
#include "buttons.h"

// Global Variables
// -----------------
static uint32_t enter, up, down, prev_enter = 0;

// Initialization
// --------------
void BUTTONS_Init(void){
    GPIO_PORTE_DIR_R &= ~( (1<<0) | (1<<2) | (1<<3) );          // Inputs
    GPIO_PORTE_PDR_R |= (1<<0) | (1<<2) | (1<<3);               // Enable pull down
    GPIO_PORTE_AFSEL_R &= ~((1<<0) | (1<<2) | (1<<3));          // Disable analog function
    GPIO_PORTE_DEN_R |= (1<<0) | (1<<2) | (1<<3);               // Enable digital function
}

// Polling over buttons
// --------------------
void BUTTONS_Check(void){
    uint8_t max;
    enter = GPIO_PORTE_DATA_R&(1<<0);
    up = GPIO_PORTE_DATA_R&(1<<2);
    down = GPIO_PORTE_DATA_R&(1<<3);

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
