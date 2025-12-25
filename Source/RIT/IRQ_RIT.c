/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{
	static int tick = 0;						// Quando arriva a 20 significa che è scattato un secondo
	
	// Variabili di stato che mi servono per notare se il pulsante è stato premuto
	static int up = 0;							
	static int right = 0;
	static int left = 0;
	static int down = 0;
	
	
	// Timer caduta 1 s
	tick++;
	if (tick == 20){	
		tick = 0;											// Azzero contatore
		/*---Gestione movimento del tetrocazzo----*/
	}
	
	//---- Gestione joystick ----
	
	// Joystick UP
	if ((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		up++;
		switch(up){
			case 1:
				//Implementare rotazione di 90°
				break;
			default:
				break;
	}
		
	// Joystick RIGHT
	if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		up++;
		switch(up){
			case 1:
				//Implementare shift a dx
				break;
			default:
				break;
	}
		
	// Joystick LEFT
	if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		up++;
		switch(up){
			case 1:
				//Implementare shift a sx
				break;
			default:
				break;
	}
		
	// Joystick DOWN
	if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		up++;
		switch(up){
			case 1:
				//Implementare soft drop
				break;
			default:
				break;
	}
		
	// Gestione dei pulsanti
	
	//Pulsante EINT1
	if(down>=1)
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
				switch(down){
					case 2:
						// qualcosa
						break;
					default:
						break;
				}
				down++;
			}
			else{
				down = 0;
				LPC_PINCON->PINSEL4 |= (1 << 22);
				NVIC_EnableIRQ(EINT1_IRQn);
			}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
