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

volatile int k1down = 0;
volatile int k2down = 0;

void RIT_IRQHandler (void)
{
	static int tick = 0;						// Quando arriva a 20 significa che è scattato un secondo
	int new_speed = 20;							// Nuovo valore del limite da raggiungere (usato per joy down)
		
	// Variabili di stato che mi servono per notare se il pulsante è stato premuto
	static int up = 0;							
	static int right = 0;
	static int left = 0;
	
		
	// Joystick DOWN 
	if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		new_speed = 10;
	}
	else {
		// Altrimenti velocità normale
	}
	
	
	// Timer caduta 1s
	tick++;
	if (tick >= new_speed){	
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
	}
	else {
		up = 0;
	}
		
	// Joystick RIGHT
	if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		right++;
		switch(right){
			case 1:
				//Implementare shift a dx
				break;
			default:
				break;
		}
	}
	else {
		right = 0;
	}
		
	// Joystick LEFT
	if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		left++;
		switch(left){
			case 1:
				//Implementare shift a sx
				break;
			default:
				break;
		}
	}
	else {
		left = 0;
	}
	
		
	/* ---- GESTIONE DEI PULSANTI ----- */
	
	// Pulsante EINT1
	if(k1down>=1){
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){								// Controlla se il tasto è ancora premuto fisicamente
				switch(k1down){
					case 2:																						// case 2 -> down = 2 -> 100ms (= 50*2) come tempo di sicurezza
						// Implementazone tasto 1 (pausa/ripresa del gioco)
						break;
					default:
						break;
				}
				k1down++;
			}
			else{
				k1down = 0;																						// Rimetto a 0 la flag
				LPC_PINCON->PINSEL4 |= (1 << 22);											// Rimetto il pin come EINT
				NVIC_EnableIRQ(EINT1_IRQn);														// Riaccendo l'interrupt
			}
	}
	
	// Pulsante EINT2
	if(k2down>=1){
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){								// Controlla se il tasto è ancora premuto fisicamente
				switch(k2down){
					case 2:																						// case 2 -> down = 2 -> 100ms (= 50*2) come tempo di sicurezza
						// Implementazone caduta istantanea
						break;
					default:
						break;
				}
				k2down++;
			}
			else{
				k2down = 0;																					// Rimetto a 0 la flag
				LPC_PINCON->PINSEL4 |= (1 << 24);										// Rimetto il pin come EINT
				NVIC_EnableIRQ(EINT2_IRQn);													// Riaccendo l'interrupt
			}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
