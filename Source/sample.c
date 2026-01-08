/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "tetris/tetris.h"
#include "button.h"
#include "joystick/joystick.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

extern volatile uint8_t reset_requested;

volatile float periodo = 0.01; //sec

volatile int hard_drop_requested = 0; // 1 quando l'utente preme il tasto

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	//LCD_Clear(Black);
	BUTTON_init();
	joystick_init();
  //TP_Init();
	//TouchPanel_Calibrate();

	
	tetrisInit();
	
	/* 3) Repetitive Interrupt Timer (RIT) */
	//init_RIT(0x05F5E100);									/* RIT initialization 1 sec */
	init_RIT(periodo*25000000);									/* RIT initialization */
	enable_RIT();

	
	//LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	//LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		if (reset_requested) {
            
            reset_requested = 0; // Abbassiamo la bandierina
            
            // Re-inizializza tutto (pulisce schermo, matrice, punteggio)
            tetrisInit();         
            
            // Resetta lo stato di Game Over
            gameover_flag = 0;    
            
            // Fa ripartire il gioco
            state_game = 1;       
            
            // Aggiorna la scritta di stato
            GUI_Text(160, 220, (uint8_t *) "INGAME", Green, Black);
    }
		
		if (state_game) {
			if (gravity_tick) {
					handle_gravity();
					gravity_tick = 0; // Reset
			}
	
			if (hard_drop_requested) {
					execute_hard_drop_logic(); 
					hard_drop_requested = 0; // Reset del flag
			}
			
			if (rotation_requested) {
							execute_rotation_logic();
							rotation_requested = 0;
					}
			if (move_right_requested) {
							handle_right_movement();
							move_right_requested = 0;
			}
			if (move_left_requested) {
							handle_left_movement();
							move_left_requested = 0;
			}
		}
    //__ASM("wfi"); // Risparmio energetico
  }
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
