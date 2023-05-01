/*-----------------------------------------------
 * File:        screen.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Source file of LCD updater.
 *---------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "int_port.h"
#include "keypad/keypad.h"
#include "lcd/lcd.h"
#include "screen.h"
#include "adc/adc.h"

// Global Variables
// -----------------
uint8_t text[5], screen = 1;
uint8_t* speed_text, * max_text, * t;

// Takes sample from ADC (0 to 4095)
// Returns motor speed in decimal from 0 to 100
// --------------------------------------------
uint8_t get_speed(void){
    uint32_t speed;
    float f_speed;
    speed = convert();
    f_speed = M_RELOAD - (float)speed;
    f_speed /= M_RELOAD;
    f_speed *= 100;
    if(f_speed>=99.5){                      //Because minimum is 1600
        f_speed = ceil(f_speed);
    }
    return (uint8_t)f_speed;
}

// Convert to text
// ---------------
uint8_t* to_text(uint8_t num){
    if(num<10){
        text[0] = num + '0';
        text[1] = '%';
        text[2] = ' ';
        text[3] = ' ';
        text[4] = '\0';
    }
    else if(num>9 && num <100){
        text[0] = (num/10) + '0';
        text[1] = (num%10) + '0';
        text[2] = '%';
        text[3] = ' ';
        text[4] = '\0';
    }
    else if(num == 100){
        text[0] = '1';
        text[1] = '0';
        text[2] = '0';
        text[3] = '%';
        text[4] = '\0';
    }
    return text;
}

// Update text on screen
// ---------------------
void update_screen(void){
    uint32_t s;
    uint8_t m;
    if(screen ==1){                                 // Main screen
        s = get_speed();
        speed_text = to_text(s);
        LCD_Text(1, 1, "S:");                       // Value of motor speed
        LCD_Text(1, 3, speed_text);

        m = get_max();
        max_text = to_text(m);
        LCD_Text(1, 8, "M:");                       // Value of maximum speed
        LCD_Text(1, 10, max_text);
        LCD_Text(2, 1, "Enter:Change MAX");

    }
    else if(screen == 2){                           // Screen to change max speed with
        m = get_max();
        max_text = to_text(m);
        LCD_Text(1, 1,"Up/Down:");
        LCD_Text(2, 1,"Enter:Set&Back");
        LCD_Text(1, 9,max_text);
    }
}


