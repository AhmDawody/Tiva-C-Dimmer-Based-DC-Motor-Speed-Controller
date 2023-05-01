/*-----------------------------------------------
 * File:        lcd.c
 * Author:      Ahmed Shehata
 * Date:        March, 2023
 * Description: Source file of LCD module.
 *---------------------------------------------*/

// Included Header Files
// ----------------------
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay/delay.h"
#include "inc/tm4c123gh6pm.h"

// Global Variables
// -----------------
static const uint8_t COMMAND = 0;
static const uint8_t DATA = 1;
static const uint8_t DATA_PINS = 0xF0;
static const uint8_t CTRL_PINS = 0x0C;

// Functions Prototypes
// ---------------------
static void MCU_Pins_Config(void);
static void LCD_Send(uint8_t, uint8_t);



void LCD_Init(void){
    MCU_Pins_Config(); //1-Port initialization
    Sys_Init();
    Sys_Wait1ms(20);
    //2- 0x03
    LCD_Send(0x00,COMMAND); //upper nibble
    LCD_Send(0x03,COMMAND); //lower nibble
    Sys_Wait1ms(5);
    //3- 0x03
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x03,COMMAND);
    Sys_Wait1Us(200);
    //4- 0x03
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x03,COMMAND);
    Sys_Wait1Us(200);
    //5- 0x02
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x02,COMMAND);
    Sys_Wait1Us(200);
    //6- 0b0010 1000 -> 2line, 5*7, 4bit
    LCD_Send(0x02,COMMAND);
    LCD_Send(0x08,COMMAND);
    Sys_Wait1Us(200);
    //7- 0b0000 1000 -> display off
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x08,COMMAND);
    Sys_Wait1Us(200);
    //8- 0b0000 0001 -> display clear
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x01,COMMAND);
    Sys_Wait1ms(2);
    //9- 0b0000 0110 -> cursor move right
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x06,COMMAND);
    Sys_Wait1Us(200);
    //10- 0b0000 1100 -> display on cursor off
    LCD_Send(0x00,COMMAND);
    LCD_Send(0x0C,COMMAND);
    Sys_Wait1Us(200);
}


static void MCU_Pins_Config(void){
    SYSCTL_RCGC2_R |= (LCD_DATA_PORT_ENABLE | LCD_CONTROL_PORT_ENABLE);
    while((SYSCTL_RCGCGPIO_R & 0x05) == 0);

    GPIO_PORTC_DIR_R |= DATA_PINS;
    GPIO_PORTA_DIR_R |= CTRL_PINS;

    GPIO_PORTC_DATA_R &= ~DATA_PINS;
    GPIO_PORTA_DATA_R &= ~CTRL_PINS;

    GPIO_PORTC_AFSEL_R &= ~DATA_PINS;
    GPIO_PORTA_AFSEL_R &= ~CTRL_PINS;

    GPIO_PORTC_DEN_R |= DATA_PINS;
    GPIO_PORTA_DEN_R |= CTRL_PINS;
}

static void LCD_Send(uint8_t value, uint8_t type){
    uint8_t tmp = 0;

    if(type == COMMAND)
        GPIO_PORTA_DATA_R &= ~LCD_RS;
    else
        GPIO_PORTA_DATA_R |= LCD_RS;

    tmp =(value>>0) & 1;
    if(tmp)
        GPIO_PORTC_DATA_R |= LCD_D4;
    else
        GPIO_PORTC_DATA_R &= ~LCD_D4;

    tmp =(value>>1) & 1;
    if(tmp)
        GPIO_PORTC_DATA_R |= LCD_D5;
    else
        GPIO_PORTC_DATA_R &= ~LCD_D5;

    tmp =(value>>2) & 1;
    if(tmp)
        GPIO_PORTC_DATA_R |= LCD_D6;
    else
        GPIO_PORTC_DATA_R &= ~LCD_D6;

    tmp =(value>>3) & 1;
    if(tmp)
        GPIO_PORTC_DATA_R |= LCD_D7;
    else
        GPIO_PORTC_DATA_R &= ~LCD_D7;

    Sys_Wait1Us(1);

    GPIO_PORTA_DATA_R |= LCD_EN;
    Sys_Wait1Us(1);
    GPIO_PORTA_DATA_R &= ~LCD_EN;
    Sys_Wait1Us(4);
}

void LCD_Cmd(uint8_t cmd){
    uint32_t val =0, tmp =0;
    if(cmd == LCD_CLEAR){
        LCD_Send(0x0, COMMAND); //upper nibble
        LCD_Send(0x1, COMMAND); //lower nibble
        Sys_Wait1ms(2);
        return;
    }
    else if(cmd ==LCD_CURSOR_OFF)
        val = 0x0C; //0b00001100
    else if(cmd ==LCD_UNDERLINE_ON)
        val = 0x0E; //0b00001110
    else if(cmd ==LCD_BLINK_CURSOR_ON)
        val = 0x0D; //0b00001101
    else if(cmd ==LCD_FIRST_ROW)
        val = 0x80; //0b10000000
    else if (cmd == LCD_CURSOR_LEFT)
        val = 0x04;  //0b00000100
    else    //second line
        val = 0x80 + 0x40; //0b10000000 +0x40(second line)

    tmp = val>>4;
    LCD_Send(tmp, COMMAND); //upper nibble
    LCD_Send(val, COMMAND); //lower nibble
    Sys_Wait1Us(200);
}

void Set_Address(uint8_t row, uint8_t col){
    uint32_t address =0,tmp =0;

    if((row ==1)||(row ==LCD_FIRST_ROW))
        address = 0x80 + (col-1);
    else
        address = (0x80+0x40) + (col-1);

    tmp = address>>4;
    LCD_Send(tmp,COMMAND);
    LCD_Send(address,COMMAND);
    Sys_Wait1Us(200);
}

void LCD_Chr_Cp(uint8_t chr){
    uint32_t tmp =0;

    tmp = chr>>4;
    LCD_Send(tmp,DATA);
    LCD_Send(chr,DATA);
    Sys_Wait1ms(10);
}

void LCD_Chr(uint8_t row, uint8_t col, uint8_t chr){
    Set_Address(row, col);
    LCD_Chr_Cp(chr);
}

void LCD_Text_Cp(uint8_t* text){
    while(*text!='\0'){
        LCD_Chr_Cp(*text);
        text ++;
    }
}

void LCD_Text(uint8_t row, uint8_t col, uint8_t* text){
    Set_Address(row, col);
    LCD_Text_Cp(text);
}
















