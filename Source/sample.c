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
//#include "Tetrami_function/Tetra_function.h"

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
   uint16_t x;
   uint16_t y;
}Coord_str;

void LCD_tetraminoes(uint8_t shape, Coord_str xy, uint8_t set);		//if set is equal to 1 it draw the tetraminoes otherwise it erase it
void tetrisInit();
void update_score(uint16_t score, uint16_t topScore, uint16_t lines);
void uint16_to_ascii_uint8(uint16_t val, uint8_t *dest);

uint16_t score = 0;
uint16_t topScore = 0;
uint16_t lines = 0;


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
  //TP_Init();
	//TouchPanel_Calibrate();
	LCD_Clear(Black);
	
	tetrisInit();
	
	int i = 0;
	
	Coord_str coord;
	
	int dim = 14;	//dimension of foundamental block (square 14px * 14px)
	
	//----START: DRAW FOUNDAMENTAL BOCK----
	for(i=0;i<dim;i++){
		LCD_DrawLine(50, 150+i, (50+dim), 150+i, White);
	}
	//----END: DRAW FOUNDAMENTAL BOCK----
	
	coord.x = 100;
	coord.y = 250;
	
	LCD_tetraminoes(S_Shaped, coord, 1);
	
	
	
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


void tetrisInit(){
	
	int i = 0;

	//----START: DRAW PLAYING FIELD----
	for(i=0;i<3;i++){
		LCD_DrawLine(i, 20, i, 319, White);
		LCD_DrawLine(0, 319-i, 150, 319-i, White);
		LCD_DrawLine(150-i, 319, 150-i, 20, White);
		LCD_DrawLine(150, 20+i, 0, 20+i, White);
	}
	//----END: DRAW PLAYING FIELD----
	
	update_score(0,0,0);

}

/******************************************************************************
* Function Name  : update_score
* Description    : this function update the written value of the current score, highest score and deleted lines on the display
* Input          : - score: value of score
*                  - topScore: value of highest score
*									 - lines: value of deleted lines
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void update_score(uint16_t score, uint16_t topScore, uint16_t lines){
	
	uint8_t score_char_ptr[6];
	uint8_t topScore_char_ptr[6];
	uint8_t lines_char_ptr[6];
	
	uint16_to_ascii_uint8(score, score_char_ptr);
	uint16_to_ascii_uint8(topScore, topScore_char_ptr);
	uint16_to_ascii_uint8(lines, lines_char_ptr);
	
	GUI_Text(160, 40, (uint8_t *) "SCORE", Red, Black);
	GUI_Text(160, 55, (uint8_t *) score_char_ptr, White, Black);
	
	GUI_Text(160, 100, (uint8_t *) "TOP SCORE", Red, Black);
	GUI_Text(160, 115, (uint8_t *) topScore_char_ptr, White, Black);

	GUI_Text(160, 160, (uint8_t *) "LINES", Red, Black);
	GUI_Text(160, 175, (uint8_t *) lines_char_ptr, White, Black);

}


/**
 * Converts a uint16_t to ASCII in a uint8_t buffer.
 * Max value 65535 requires a 6-byte buffer.
 */
void uint16_to_ascii_uint8(uint16_t val, uint8_t *dest) {
    // We work from the end of the potential max string length
    // [d1][d2][d3][d4][d5][\0]
    //  0   1   2   3   4   5  <- Index
    
    uint8_t buffer[5]; 
    int8_t i = 4;

    if (val == 0) {
        dest[0] = '0';
        dest[1] = '\0';
        return;
    }

    // Decompose number into digits from right to left
    while (val > 0 && i >= 0) {
        buffer[i--] = (uint8_t)(val % 10) + '0';
        val /= 10;
    }

    // 'i' now points to the index before the first digit
    // We shift the result to the start of the dest pointer
    uint8_t j = 0;
		uint8_t k = 0;
    for (k = i + 1; k <= 4; k++) {
        dest[j++] = buffer[k];
    }
    
    dest[j] = '\0'; // Null-terminate
}

/******************************************************************************
* Function Name  : LCD_tetraminoes
* Description    : this function draw/erase on the display the selected tetramino
* Input          : - shape: selected ttraminoes
*                  - xy: coordinates (top-left) of the tetramino
*									 - set: if 1 it draw the tetramino, if 0 it erase the tetramino
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void LCD_tetraminoes(uint8_t shape, Coord_str xy, uint8_t set){
	
	int i = 0;
	int l = 0;
	uint16_t x0 = xy.x;
	uint16_t y0 = xy.y;
	uint16_t color = Black;
	
	int dim = 14;	//dimension of foundamental block (square 14px * 14px)
	
	if (set){
		if(shape == I_Shaped) color = Cyan;
		if(shape == T_Shaped) color = Magenta;
		if(shape == Z_Shaped) color = Red;
		if(shape == O_Shaped) color = Yellow;
		if(shape == J_Shaped) color = Blue;
		if(shape == L_Shaped) color = Grey;
		if(shape == S_Shaped) color = Green;
	}
		
	
	switch (shape) {
	
		case I_Shaped:
			//----START: DRAW I-SHAPED----
			for(l=0;l<(dim*3)+1;l = l+dim){
				for(i=0;i<dim;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}
			}
			break;
			//----END: DRAW I-SHAPED----
	
		case T_Shaped:
			//----START: DRAW T-SHAPED----
			for(l=0;l<(dim*2)+1;l = l+dim){
				if(l!=dim){
				for(i=0;i<dim;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}}else{
				
				for(i=0;i<dim*2;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}}
				
			}
			break;
			//----END: DRAW T-SHAPED----
			
		case Z_Shaped:
			//----START: DRAW Z-SHAPED----
				for(l=0;l<(dim*2)+1;l = l+dim){ //x-axe
					if(!l){
					for(i=0;i<dim;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}}else if (l==dim){
					
					for(i=0;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}}else if (l==2*dim){
					
					for(i=dim;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}}
					
				}
				break;
			//----END: DRAW Z-SHAPED----
				
		case O_Shaped:
			//----START: DRAW O-SHAPED----
			for(l=0;l<(dim)+1;l = l+dim){
				for(i=0;i<dim*2;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}
			}
			//----END: DRAW O-SHAPED----
		break;
		case J_Shaped:
			//----START: DRAW J-SHAPED----
			for(l=0;l<(dim)+1;l = l+dim){
				if(!l){
				for(i=dim*2;i<(dim*3)+1;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}}else{
				
				for(i=0;i<(dim*3)+1;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}}
				
			}
			//----END: DRAW J-SHAPED----
		break;
		case L_Shaped:
			//----START: DRAW L-SHAPED----
			for(l=0;l<(dim)+1;l = l+dim){
				if(!l){
					for(i=0;i<(dim*3)+1;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				}
				}else{
					for(i=dim*2;i<(dim*3)+1;i++){
					LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
				
				}}
				
			}
			//----END: DRAW L-SHAPED----
		break;
		case S_Shaped:
			//----START: DRAW S-SHAPED----
				for(l=0;l<(dim*2)+1;l = l+dim){ //x-axe
					if(!l){
					for(i=dim;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}
					}else if (l==dim){
					
					for(i=0;i<dim*2;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}
					}else if (l==2*dim){
					for(i=0;i<dim;i++){
						LCD_DrawLine(x0+l, y0+i, (x0+dim)+l, y0+i, color);
					}
					
					}
					
				}
			//----END: DRAW S-SHAPED----
		break;
		default:
        GUI_Text(x0, y0, (uint8_t *) "MISSING TETRAMINOES", Red, Black);
        break;
	}

}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
