/*-------------------------------------------------------------------
 * File:        int_port.h
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Header file of port a used to capture signals from zero
 * crossing circuit and triggers triac after certain delay.
 *---------------------------------------------------------------------*/
#ifndef INT_PORT_H_
#define INT_PORT_H_

// Global Variables
// -----------------
extern volatile uint32_t reload;
extern volatile uint32_t sample;

// Function Prototypes
// --------------------
void PortA_Init(void);
void Trigger(void);

#endif /* INT_PORT_H_ */
