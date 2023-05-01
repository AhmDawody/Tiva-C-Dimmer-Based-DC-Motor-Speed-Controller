/*-----------------------------------------------
 * File:        adc.c
 * Author:      Ahmed Shehata
 * Date:        March, 2023
 * Description: Source file of the ADC module .
 *---------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "adc.h"
#include "I2C/I2C.h"

// Define macros
// -------------
#define adc_pin 1

// Global Variables
// -----------------
uint8_t max;


// Initialization
// --------------
void ADCInit(void){
    SYSCTL_RCGCADC_R |= (1<<0);                         //enable clock for adc0
    while((SYSCTL_RCGCADC_R&0x1)==0);                   //Wait
    SYSCTL_RCGCGPIO_R |= (1<<4);                        //Enable port e clock
    while((SYSCTL_RCGCGPIO_R&(1<<4))==0);               //Wait
    GPIO_PORTE_DIR_R &= ~(1<<adc_pin);                  //Input
    GPIO_PORTE_AFSEL_R |= (1<<adc_pin);                 //Enable alternate function
    GPIO_PORTE_DEN_R &= ~(1<<adc_pin);                  //Disable digital function
    GPIO_PORTE_AMSEL_R |= (1<<adc_pin);                 //Enable analog function
    ADC0_SSPRI_R|=(0<<12)|(0x1<<8)|(0x2<<4)|(0x3<<0);   //SS3 Highest, SS0 Lowest
    ADC0_ACTSS_R &= ~(1<<3);                            //SS3 is disabled during initialization
    ADC0_EMUX_R &= ~(0xF<<12);                          //SS3 is processor triggered
    ADC0_SSMUX3_R = (0x2);                              //Channel Ain2
    ADC0_SSCTL3_R = 0x6;                                //No TS0 D0, yes IE0 END0
    ADC0_ACTSS_R |= (1<<3);                             //SS3 is enabled
}

// Sample Capture
// --------------
uint32_t ADC_IN_AIN1(void){
    uint32_t r;
    ADC0_PSSI_R |= (1<<3);                              //Initialize SS3
    while((ADC0_RIS_R&(1<<3))==0);                      //wait for 1
    r = ADC0_SSFIFO3_R&0xFFF;                           //result in r
    ADC0_ISC_R |= (1<<3);                               //clear ss3 by writing 1 to ISC
    return r;
}

/**********************************************
 * Takes ADC sample, value from 0 to 4095
 * 0 -> stationary, 4095 -> maximum speed
 * Stationary -> max reload value (M_RELOAD)
 * Maximum speed -> minimum reload value (1600)
 **********************************************/
uint32_t convert(void){
    uint32_t adc_read;
    uint8_t m;
    float sample;
    m = get_max();
    adc_read = ADC_IN_AIN1();
    sample = (float)adc_read /4095;
    sample = sample * m / 100;
    sample *= M_RELOAD;
    sample = M_RELOAD - sample;
    if(sample<1600){                                    // Minimum threshold = 20us
        sample = 1600;
    }
    return (uint32_t)sample;
}


uint8_t get_max(void){
    return max;
}

// Takes max value from EEPROM
// Set max to this value
// ---------------------------
void Update_max(void){
    volatile uint8_t data;
    data = I2C_Recieve();
    max = data;
}

void inc_max(void){
    if(max < 100){
        max++;
    }
}

void dec_max(void){
    if(max > 1){
        max--;
    }
}
