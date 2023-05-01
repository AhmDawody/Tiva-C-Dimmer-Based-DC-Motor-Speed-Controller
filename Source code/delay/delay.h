/*-------------------------------------------------------------------
 * File:        delay.h
 * Author:      Ahmed Shehata
 * Date:        March, 2023
 * Description: Header file of the Systick timer module .
 *------------------------------------------------------*/

#ifndef DELAY_H_
#define DELAY_H_


#include <stdint.h>
#include <stdbool.h>



void Sys_Init(void);
void Sys_Wait(uint32_t);
void Sys_Wait1ms(uint32_t);
void Sys_Wait10ms(uint32_t);
void Sys_Wait1Us(uint32_t);


#endif /* DELAY_H_ */
