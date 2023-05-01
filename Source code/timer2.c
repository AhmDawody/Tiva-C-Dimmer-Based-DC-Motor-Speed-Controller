/*-------------------------------------------------------------------
 * File:        timer2.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Source file of timer 2 used to trigger triac
 *---------------------------------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "timer2.h"
#include "lcd/lcd.h"
#include "screen.h"
#include "adc/adc.h"
#include "int_port.h"
#include "delay/delay.h"

// Initialization function
// -----------------------
void Timer2_Init(){
  uint32_t volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;                               // Enable timer clock
  delay = SYSCTL_RCGCTIMER_R;                               // Wait
  TIMER2_CTL_R = 0x00000000;                                // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;                                // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000001;                               // 3) configure for one-shot mode, default down-count settings
  TIMER2_TAPR_R = 0;                                        // 4) bus clock resolution
  TIMER2_ICR_R = 0x00000001;                                // 5) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;                                // 6) arm timeout interrupt
  NVIC_PRI5_R |= (NVIC_PRI5_R&0x00FFFFFF)|0x80000000;       // 7) priority 4
  NVIC_EN0_R = 1<<23;                                       // 8) enable IRQ 23 in NVIC, vector number 39
}

// Timer2A ISR
// -----------
void Timer2A_Handler(void){
    Trigger();
    TIMER2_ICR_R = 0x00000001;                              // Acknowledge
}

// Reload the timer
// ----------------
void Timer2A_Start(uint32_t period){
  TIMER2_CTL_R |= 0x00000001;                               // Enable timer
  TIMER2_TAILR_R = period-1;                                // Reload value
}

// Stop the timer
// --------------
void Timer2A_Stop(void){
  TIMER2_CTL_R &= ~0x00000001; // disable
}

