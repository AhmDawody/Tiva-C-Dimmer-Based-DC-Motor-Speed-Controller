/*-------------------------------------------------------------------
 * File:        delay.c
 * Author:      Ahmed Shehata
 * Date:        March, 2023
 * Description: Source file of the Systick timer module .
 *-------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "delay.h"
#include "inc/tm4c123gh6pm.h"

//**********************************clock 16MHz or 80MHz****************************************
// Reload = (80M or 16M) * time
#define wait_in_ms 80000
#define wait_in_10ms 800000
#define wait_in_Us 80

void Sys_Init(void){
    NVIC_ST_CTRL_R = 0x00;
    NVIC_ST_CTRL_R = 0x05;
}

void Sys_Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay - 1;
    NVIC_ST_CURRENT_R = 0x00;
    while((NVIC_ST_CTRL_R&0x10000)==0);
}

void Sys_Wait1ms(uint32_t delay){
    uint32_t i;
    for(i=0; i<delay; i++){
        Sys_Wait(wait_in_ms);
    }
}

void Sys_Wait10ms(uint32_t delay){
    uint32_t i;
    for(i=0; i<delay; i++){
        Sys_Wait(wait_in_10ms);
    }
}

void Sys_Wait1Us(uint32_t delay){
    uint32_t i;
        for(i=0; i<delay; i++){
            Sys_Wait(wait_in_Us);
        }
}
