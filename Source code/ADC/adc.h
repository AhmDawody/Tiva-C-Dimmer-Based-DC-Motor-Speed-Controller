/*-------------------------------------------------------------------
 * File:        adc.h
 * Author:      Ahmed Shehata
 * Date:        March, 2023
 * Description: Header file of the ADC module .
 *---------------------------------------------*/
#ifndef ADC_H_
#define ADC_H_

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>

// Define macros
// -------------
#define M_RELOAD 788000     //9850us

// Global Variables
// -----------------
extern uint8_t max;

// Functions Prototypes
// ---------------------
void ADCInit(void);
uint32_t ADC_IN_AIN1(void);
uint32_t convert(void);
uint8_t get_max(void);
void Update_max(void);
void inc_max(void);
void dec_max(void);

#endif /* ADC_H_ */
