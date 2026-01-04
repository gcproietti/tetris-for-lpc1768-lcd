#ifndef __TETRIS_H
#define __TETRIS_H

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 

#ifndef i
extern uint16_t i;
#endif

#ifndef j
extern uint16_t j;
#endif


#define dim 14	//dimension of foundamental block (square 14px * 14px)

/**DEFINIZIONE DEI TETRAMINI SU MATRICI */
extern uint8_t tetramino_I[4][4];  
extern uint8_t tetramino_O[4][4];
extern uint8_t tetramino_T[4][4];
extern uint8_t tetramino_S[4][4];
extern uint8_t tetramino_Z[4][4];
extern uint8_t tetramino_J[4][4];
extern uint8_t tetramino_L[4][4];


typedef	struct  
{
   uint16_t x;
   uint16_t y;
}Coord_str;

extern uint8_t field_matrix[10][20];

void tetrisInit();
void ruota_90_gradi(uint8_t matrice[4][4], int righe, int colonne, uint8_t ruotata[4][4]);
void update_field_matrix(uint8_t matrix[4][4], Coord_str coord);
Coord_str coordinate_su_schermo(Coord_str coord_matrice);
void LCD_tetraminoes(uint8_t matrice[4][4], Coord_str xy, uint8_t set);
void update_score(uint16_t score, uint16_t topScore, uint16_t lines);
void uint16_to_ascii_uint8(uint16_t val, uint8_t *dest);
uint8_t lfsr_random();
void random_tetramino(uint8_t matrix[4][4]);

//int collision(Coord_str coord, int16_t increase_x, int16_t increase_y);

#endif
