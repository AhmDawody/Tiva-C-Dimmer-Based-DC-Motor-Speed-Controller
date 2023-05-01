/*-------------------------------------------------------------------
 * File:        timer2.h
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Header file of timer 2 used to fire the angle of triac
 *---------------------------------------------------------------------*/
#ifndef TIMER2_H_
#define TIMER2_H_

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>

// Function Prototypes
// --------------------
void Timer2_Init(void);
void Timer2A_Start(uint32_t);
void Timer2A_Stop(void);

#endif /* TIMER2_H_ */
