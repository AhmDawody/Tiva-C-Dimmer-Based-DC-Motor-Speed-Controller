/*-----------------------------------------------
 * File:        I2C.c
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Header file of I2C module.
 *---------------------------------------------*/
#ifndef I2C_H_
#define I2C_H_

// Functions Prototypes
// ---------------------
void I2C_Init(void);
void I2C_Send(uint8_t data);
uint8_t I2C_Recieve(void);

#endif /* I2C_H_ */
