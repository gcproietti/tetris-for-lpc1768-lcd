#include "button.h"
#include "lpc17xx.h"

extern int k1down;								// Variabile di stato per controllar se k1 è premuto
extern int k2down;								// Variabile di stato per controllar se k2 è premuto

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);							// Disabilito l'interrupt del bottone
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     // Trasformo il pin in GPIO
	k1down=1;																	// Salto al RIT grazie all'uso della flag
	LPC_SC->EXTINT &= (1 << 1);     					/* clear pending interrupt         */  
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn);							// Disabilito l'interrupt del bottone
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     // Trasformo il pin in GPIO
	k2down=1;																	// Salto al RIT grazie all'uso della flag
  LPC_SC->EXTINT &= (1 << 2);     					/* clear pending interrupt         */    
}