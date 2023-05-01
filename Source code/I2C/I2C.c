/*-----------------------------------------------
 * File:        I2C.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Source file of I2C module.
 *---------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "I2C.h"

// Define macros
// -------------
#define EEPROM_ADDRESS 0x50                             // Slave address
#define MEMORY_ADDRESS 64                               // Memory address to save data in
#define I2C_MCS_ACK 0x00000008                          // Data Acknowledge Enable
#define I2C_MCS_ADRACK 0x00000004                       // Acknowledge Address
#define I2C_MCS_STOP 0x00000004                         // Generate STOP
#define I2C_MCS_START 0x00000002                        // Generate START
#define I2C_MCS_ERROR 0x00000002                        // Error
#define I2C_MCS_RUN 0x00000001                          // I2C Master Enable

// Initialization
// --------------
void I2C_Init(void) {
    // Enable I2C0 module
    SYSCTL_RCGCI2C_R |= 0x1;
    while((SYSCTL_PRI2C_R & 0x1) == 0) {}

    // Enable GPIOB module
    SYSCTL_RCGCGPIO_R |= (1<<1);
    while((SYSCTL_RCGCGPIO_R&(1<<1))==0);

    // Set PB2 and PB3 as I2C0 pins
    GPIO_PORTB_AFSEL_R |= (1 << 2) | (1 << 3);
    GPIO_PORTB_ODR_R |= (1 << 3);                        // Enable open drain on SDA pin
    GPIO_PORTB_DEN_R |= (1 << 2) | (1 << 3);
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF00FF) | 0x00003300;

    // Configure I2C0 module for 100 kbps clock speed
    I2C0_MCR_R = I2C_MCR_MFE;                            // Enable master mode
    I2C0_MTPR_R = (39<<0);                               // Set SCL clock period to 100 kbps
}


void I2C_Send(uint8_t data){
    // Send start condition
    I2C0_MSA_R = (EEPROM_ADDRESS << 1) | 0;             // Set the slave address and transmit bit
    I2C0_MDR_R = MEMORY_ADDRESS;                        // Set the memory address to write to
    I2C0_MCS_R = 3;                                     // Start, run, and stop bits

    // Wait for the I2C module to finish
   while (I2C0_MCS_R & 1) {}

    // Check for errors
    if (I2C0_MCS_R & 0x02) {
        // Error occurred, handle it here
        //return;
    }

    // Send the data byte
    I2C0_MDR_R = data;
    I2C0_MCS_R = 5; // Run and stop bits

    // Wait for the I2C module to finish
    while (I2C0_MCS_R & 1) {}

    // Check for errors
    if (I2C0_MCS_R & 0x02) {
        // Error occurred, handle it here
        //return;
    }
}




uint8_t I2C_Recieve(void){
    // Send start condition
    I2C0_MSA_R = (EEPROM_ADDRESS << 1) | 0; // Set the slave address and transmit bit
    I2C0_MCS_R = 3; // Start, run, and stop bits

    // Wait for the I2C module to finish
    while (I2C0_MCS_R & 1) {}

    // Check for errors
    if (I2C0_MCS_R & 0x02) {
        // Error occurred, handle it here
        return 0;
    }

    // Send the memory address to read from
    I2C0_MDR_R = MEMORY_ADDRESS;
    I2C0_MCS_R = 3; // Run and stop bits

    // Wait for the I2C module to finish
    while (I2C0_MCS_R & 1) {}

    // Check for errors
    if (I2C0_MCS_R & 0x02) {
        // Error occurred, handle it here
        return 0;
    }

    // Send start condition again
    I2C0_MSA_R = (EEPROM_ADDRESS << 1) | 1; // Set the slave address and receive bit
    I2C0_MCS_R = 7; // Start, run, ack, run, nack, and stop bits

    // Wait for the I2C module to finish
    while (I2C0_MCS_R & 1) {}

    // Check for errors
    if (I2C0_MCS_R & 0x02) {
        // Error occurred, handle it here
        return 0;
    }

    // Read the received byte
    uint8_t data = I2C0_MDR_R;

    // Return the received byte
    return data;
}
