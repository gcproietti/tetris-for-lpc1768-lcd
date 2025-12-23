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


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

/* Tetraminoes Shapes  */
#define I_Shaped		0x0
#define O_Shaped		0x1
#define T_Shaped		0x2
#define J_Shaped		0x3
#define L_Shaped		0x4
#define S_Shaped		0x5
#define Z_Shaped		0x6

typedef	struct  
{
   uint8_t x;
   uint8_t y;
}Coord_str;

void LCD_tetraminoes(uint8_t shape, Coord_str xy);


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
  //TP_Init();
	//TouchPanel_Calibrate();
	LCD_Clear(Black);
	
	int i = 0;
	int l = 0;
	
	Coord_str coord;
	
	//----START: DRAW PLAYING FIELD----
	for(i=0;i<3;i++){
		LCD_DrawLine(i, 20, i, 319, White);
		LCD_DrawLine(0, 319-i, 150, 319-i, White);
		LCD_DrawLine(150-i, 319, 150-i, 20, White);
		LCD_DrawLine(150, 20+i, 0, 20+i, White);
	}
	//----END: DRAW PLAYING FIELD----
	
	GUI_Text(160, 40, (uint8_t *) "SCORE", Red, Black);
	GUI_Text(160, 120, (uint8_t *) "TOP SCORE", Red, Black);
	GUI_Text(160, 200, (uint8_t *) "LINES", Red, Black);
	
	int dim = 14;	//dimension of foundamental block (square 14px * 14px)
	
	//----START: DRAW FOUNDAMENTAL BOCK----
	for(i=0;i<dim;i++){
		LCD_DrawLine(50, 150+i, (50+dim), 150+i, White);
	}
	//----END: DRAW FOUNDAMENTAL BOCK----
	
	coord.x = 100;
	coord.y = 250;
	
	LCD_tetraminoes(T_Shaped, coord);
	
	
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	//enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

void LCD_tetraminoes(uint8_t shape, Coord_str xy){
	
		int i = 0;
		int l = 0;
		uint8_t x0 = xy.x;
		uint8_t y0 = xy.y;
		
		int dim = 14;	//dimension of foundamental block (square 14px * 14px)
	
	switch (shape) {
	
		case I_Shaped:
			//----START: DRAW I-SHAPED----
			for(l=0;l<(dim*3)+1;l = l+dim){
				for(i=0;i<dim;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
				}
			}
			break;
			//----END: DRAW I-SHAPED----
	
		case T_Shaped:
			//----START: DRAW T-SHAPED----
			for(l=0;l<(dim*2)+1;l = l+dim){
				if(l!=dim){
				for(i=0;i<dim;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
				}}else{
				
				for(i=0;i<dim*2;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
				}}
				
			}
			break;
			//----END: DRAW T-SHAPED----
			
		case Z_Shaped:
			//----START: DRAW Z-SHAPED----
				for(l=0;l<(dim*2)+1;l = l+dim){ //x-axe
					if(!l){
					for(i=0;i<dim;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
					}}else if (l==dim){
					
					for(i=0;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
					}}else if (l==2*dim){
					
					for(i=dim;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, Green);
					}}
					
				}
				break;
			//----END: DRAW Z-SHAPED----
		default:
        // blocco di istruzioni predefinito
        break;
	}

}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
