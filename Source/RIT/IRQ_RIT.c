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
#include "tetris/tetris.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern volatile int k1down;
extern volatile int k2down;			 
volatile int state_game = 0;   		// Gestisce lo start, pausa e ripresa del gioco (1 = play, 0 = pausa)

extern uint8_t tetramino[4][4];
extern Coord_str coord_init;


void RIT_IRQHandler (void)
{
	static int tick = 0;						// Quando arriva a 20 significa che è scattato un secondo
	int new_speed = 1;							// Nuovo valore del limite da raggiungere (usato per joy down)						

		
	// Variabili di stato che mi servono per notare se il pulsante è stato premuto
	static int up = 0;							
	static int right = 0;
	static int left = 0;
	
	
	
	// Pulsante EINT1
	if(k1down>=1){
			//if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){								// Controlla se il tasto è ancora premuto fisicamente
				//switch(k1down){
					//case 2:																						// case 2 -> down = 2 -> 100ms (= 50*2) come tempo di sicurezza
							if (state_game == 0)
									state_game = 1;
							else
									state_game = 0;
						//break;
					//default:
						//break;
				//}
				//k1down++;
			}
		else{
				k1down = 0;																						// Rimetto a 0 la flag
				LPC_PINCON->PINSEL4 |= (1 << 22);											// Rimetto il pin come EINT
				NVIC_EnableIRQ(EINT1_IRQn);														// Riaccendo l'interrupt
		}
	//}
	
	if(state_game == 1){
		
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
				/*---Gestione movimento del tetramino----*/
				LCD_tetraminoes(tetramino_I, coord_init, 0);
				coord_init.y += 7;
				LCD_tetraminoes(tetramino_I, coord_init, 1);
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
						  if(check_collision(tetramino, x+1, y) == 0) {
								LCD_tetraminoes(tetramino, coord_schermo, 0);
								x++;
								LCD_tetraminoes(tetramino, coord_schermo, 1);
							}
							else{
							}
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
							if(check_collision(current_tetramino, x-1, y) == 0) {
								LCD_tetraminoes(current_tetramino, coord_schermo, 0);
								x--;
								LCD_tetraminoes(current_tetramino, coord_schermo, 1);
							}
							else{
							}
						break;
					default:
						break;
				}
			}
			else {
				left = 0;
			}
			
				
			/* ---- GESTIONE DEI PULSANTI ----- */
	

			// Pulsante EINT2
			if(k2down>=1){
					if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){								// Controlla se il tasto è ancora premuto fisicamente
						switch(k2down){
							case 2:																						// case 2 -> down = 2 -> 100ms (= 50*2) come tempo di sicurezza
								/* Implementazone caduta istantanea
									1. conflitto = calcolo_conflitto;
									2. while(!conflitto){
												 muovi giù;
												 conflitto = calcolo_conflitto;
										}
									3.*/break;
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
	}
	else{
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
