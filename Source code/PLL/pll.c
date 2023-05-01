#include <stdint.h>
#include <stdbool.h>

#include "inc/tm4c123gh6pm.h"
#include "pll.h"



/*Initialization and Configuration
**The PLL is configured using direct register writes to the RCC/RCC2 register. If the RCC2 register
**is being used, the USERCC2 bit must be set and the appropriate RCC2 bit/field is used. The steps
**required to successfully change the PLL-based system clock are:
**1.Bypass the PLL and system clock divider by setting the BYPASS bit and clearing the USESYS
**      bit in the RCC register, thereby configuring the microcontroller to run off a "raw" clock source
**      and allowing for the new PLL configuration to be validated before switching the system clock
**      to the PLL.
**2.Select the crystal value (XTAL) and oscillator source (OSCSRC), and clear the PWRDN bit in
**      RCC/RCC2. Setting the XTAL field automatically pulls valid PLL configuration data for the
**      appropriate crystal, and clearing the PWRDN bit powers and enables the PLL and its output.
**3.Select the desired system divider (SYSDIV) in RCC/RCC2 and set the USESYS bit in RCC. The
**      SYSDIV field determines the system frequency for the microcontroller.
**4.Wait for the PLL to lock by polling the PLLLRIS bit in the Raw Interrupt Status (RIS) register.
**5.Enable use of the PLL by clearing the BYPASS bit in RCC/RCC2.*/


void PLLInit(void){
    SYSCTL_RCC2_R |= 0x80000000;                   //use rcc2
    SYSCTL_RCC2_R |= (1<<11);                    //setting BYPASS bit
    SYSCTL_RCC_R &= ~(1<<22);                   //clearing USESYS bit
    SYSCTL_RCC_R &= ~0x7C0;                     //Clear XTAL---
    SYSCTL_RCC_R |= (0x15<<6);                  //Set 16MHz XTAL
    SYSCTL_RCC2_R &= ~ 0x70;                    //use Main oscillator
    SYSCTL_RCC2_R &= ~(1<<13);                  //Clear PWRDN bit
    SYSCTL_RCC2_R |= (1<<30);                   //Enable DIV400
    SYSCTL_RCC2_R &= ~0x1FC00000;                //Clear SYSDIV 2 + SYSDIV2LSB
    SYSCTL_RCC2_R |= (4<<22);                   //SYSDIV2 = 0x02 , SYSDIV2LSB =0 ==> for 80MHz (400 / (4+1))
    SYSCTL_RCC_R |= (1<<22);                    //Use USESYS bit
    while((SYSCTL_RIS_R&(1<<6))==0);            //Polling the PLLLRIS bit
    SYSCTL_RCC2_R &= ~(1<<11);                    //Clear BYPASS2
}
