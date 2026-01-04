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
#ifndef i
uint16_t i = 0;
#endif

#ifndef j
uint16_t j = 0;
#endif

#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "tetris/tetris.h"
#include "RIT/RIT.h"
#include "button.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

/* Tetraminoes Shapes  
#define I_Shaped		0x0
#define O_Shaped		0x1
#define T_Shaped		0x2
#define J_Shaped		0x3
#define L_Shaped		0x4
#define S_Shaped		0x5
#define Z_Shaped		0x6*/


uint16_t score = 0;
uint16_t topScore = 0;
uint16_t lines = 0;

uint8_t current_tetramino[4][4];
extern Coord_str coord_init;

volatile float periodo = 0.03; //sec

int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	LCD_Clear(Black);
	BUTTON_init();
  //TP_Init();
	//TouchPanel_Calibrate();
	
	tetrisInit();
	
	/* 3) Repetitive Interrupt Timer (RIT) */
	//init_RIT(0x05F5E100);									/* RIT initialization 1 sec */
	init_RIT(periodo*25000000);									/* RIT initialization 1 sec */
	enable_RIT();
	

	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {		
		__ASM("wfi");
  }
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
