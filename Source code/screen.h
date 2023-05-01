/*-----------------------------------------------
 * File:        screen.h
 * Author:      Ahmed Shehata
 * Date:        April, 2023
 * Description: Header file of LCD updater.
 *---------------------------------------------*/
#ifndef SCREEN_H_
#define SCREEN_H_

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>

// Global Variables
// -----------------
extern uint8_t text[5], screen;
extern uint8_t* speed_text, * max_text, * t;


// Functions Prototypes
// ---------------------
void update_screen(void);
uint8_t get_speed(void);
uint8_t* to_text(uint8_t);

#endif /* SCREEN_H_ */
