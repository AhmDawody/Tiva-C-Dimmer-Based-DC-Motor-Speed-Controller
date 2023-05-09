/*-------------------------------------------------------------------
 * File:        int_port.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Source file of port a used to capture signals from zero
 * crossing circuit and triggers triac after certain delay.
 *------------------------------------------------------------------*/

// Function Prototypes
// --------------------
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "delay/delay.h"
#include "adc/adc.h"
#include "int_port.h"
#include "timer2.h"

// Global Variables
// -----------------
volatile uint32_t reload = M_RELOAD;                // Relaod starting from max value = largest delay
volatile uint32_t sample;                           // ADC sample


// Takes interrupt from PA6 each zero crossing
// Triggers PA7 after a certain delay
void PortA_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x1;                           // Enable portA clock
    while((SYSCTL_RCGCGPIO_R & 0x1) == 0);              // Wait
    GPIO_PORTA_DIR_R |= (1<<7) | (0<<6);                // PA7 out, PA6 input
    GPIO_PORTA_AFSEL_R &= ~((1<<7) | (1<<6));           // Disable analog function
    GPIO_PORTA_DEN_R |= (1<<7) | (1<<6);                // Enable digital function
    GPIO_PORTA_DATA_R &= ~(1<<7);                       // Low on start
    GPIO_PORTA_IS_R &= ~(1<<6);                         // Edge triggered interrupt (GPIO)
    GPIO_PORTA_IBE_R &= ~(1<<6);                        // Not both edges (GPIO)
    GPIO_PORTA_IEV_R &= ~(1<<6);                        // Falling edge (GPIO)***********
    GPIO_PORTA_IM_R |= (1<<6);                          // Unmask (GPIO)
    GPIO_PORTA_ICR_R |= (1<<6);                         // Clear flag(GPIO)
    NVIC_EN0_R |= (1<<0);                               // Enable interrupt by number in vectored table(NVIC) -> 0
    NVIC_PRI0_R |= (5<<5);                              // Priority 5(NVIC)
}

// PortA ISR
// Changing motor speed depending on ADC value in acceleration/deceleration way
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

// Triggers PA7
// -------------
void Trigger(void){
    volatile uint32_t i;
    GPIO_PORTA_DATA_R |= (1<<7);
    for (i = 0; i < 120; i++);                          // Wait
    GPIO_PORTA_DATA_R &= ~(1<<7);
}
