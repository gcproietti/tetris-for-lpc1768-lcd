#include "tetris.h"


/**DEFINIZIONE DEI TETRAMINI SU MATRICI */
uint8_t tetramino_I[4][4] = {
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};  
uint8_t tetramino_O[4][4] = {
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
uint8_t tetramino_T[4][4] = {
    {1, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
uint8_t tetramino_S[4][4] = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
uint8_t tetramino_Z[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
uint8_t tetramino_J[4][4] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};
uint8_t tetramino_L[4][4] = {
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};

//inital coordinates
Coord_str coord_init;

uint8_t field_matrix[10][20];
extern uint8_t current_tetramino[4][4];

void tetrisInit(){

	//----START: DRAW PLAYING FIELD----
	for(i=0;i<3;i++){
		LCD_DrawLine(i, 20, i, 319, White);
		LCD_DrawLine(0, 319-i, 150, 319-i, White);
		LCD_DrawLine(150-i, 319, 150-i, 20, White);
		LCD_DrawLine(150, 20+i, 0, 20+i, White);
	}
	//----END: DRAW PLAYING FIELD----
	// (0,20) - (150,20)
	// |						|
	// (0,319) - (150,319)
	
	update_score(0,0,0);
	
	coord_init.x = 47;
	coord_init.y = 24;
	
	// initialization of field matrix with all of the elements at zero
	for(i = 0; i < 10; i++){
		for(j = 0; j < 20; j++){
				field_matrix[i][j] = 0;
		}
	}
	
	for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
        current_tetramino[i][j] = 0;

    }
	}

}

void ruota_90_gradi(uint8_t matrice[4][4], int righe, int colonne, uint8_t ruotata[4][4]){
    //int ruotata[4][4];
    for(i = 0; i < colonne; i++){
        for(j = 0; j < righe; j++){
            ruotata[i][j] = matrice[righe - 1 - j][i];
        }
    }
}

// function that update the field matrix with the new tetraminoes
void update_field_matrix(uint8_t matrix[4][4], Coord_str coord){



}

Coord_str coordinate_su_schermo(Coord_str coord_matrice){
    Coord_str coord_schermo;
    coord_schermo.x = coord_matrice.x * dim; // supponendo che ogni cella sia 14 pixel di larghezza
    coord_schermo.y = coord_matrice.y * dim; // supponendo che ogni cella sia 14 pixel di altezza
    return coord_schermo;
}


/******************************************************************************
* Function Name  : LCD_tetraminoes
* Description    : this function draw/erase on the display the selected tetramino
* Input          : - matrice: matrix with the pattern of the tetraminoe
*                  - xy: coordinates (top-left) of the tetramino
*									 - set: if 1 it draw the tetramino, if 0 it erase the tetramino
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void LCD_tetraminoes(uint8_t matrice[4][4], Coord_str xy, uint8_t set){
	
	int i = 0;
	int l = 0;
	int h = 0;
	uint16_t x0 = xy.x;
	uint16_t y0 = xy.y;
	uint16_t color = Black;

	if (set)color = Yellow;

	for(l=0;l<4;l++){	//row
		for(i=0;i<4;i++){	//column
			
			if(matrice[l][i]){
				for(h=0;h<dim+1;h++){
					LCD_DrawLine(x0+i*dim, y0+l*dim+h, (x0+i*dim)+dim, y0+l*dim+h, color);
				}
			}
			
		}
	}

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


/******************************************************************************
* Function Name  : uint16_to_ascii_uint8
* Description    : this function convert a 16-bit integer in a 8-bit poiter in ASCII standard
* Input          : - val: 16-bit integer 
* Output         : - *dest: pointer to ASCII converted string
* Return         : None
* Attention		   : None
*******************************************************************************/
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
* Function Name  : lfsr_random
* Description    : this function generate a (semi)random number between 0 and 6 using the LFSR algorithm
* Input          : None
* Output         : None
* Return         : 8-bit unsigned integer
* Attention		   : None
*******************************************************************************/
uint8_t lfsr_random() {
    static uint8_t state = 0xAC;    //initial seed
		uint8_t remainder = 0x0;
    uint8_t bit;

		if (state == 0) {
			GUI_Text(0, 0, (uint8_t *) "funct lfsr_random(): ERROR", Red, Black);
				state = 0xAC; 
    }
    
		// calculate the feedback bit using XOR logic (Tap bits: 7, 5, 4, 3)
    bit  = ((state >> 7) ^ (state >> 5) ^ (state >> 4) ^ (state >> 3)) & 1;
    
    // right-shift by 1 and adding the calculated bit from left (shift to the right output)
    state = (state >> 1) | (bit << 7);
    
    remainder = state % 7;

    return remainder;
}


void random_tetramino(uint8_t matrix_rand[4][4]){
	static uint8_t num = 7;
	num = lfsr_random();
	
	
	uint8_t temp_char_ptr[6];
	uint16_to_ascii_uint8(num, temp_char_ptr);
	GUI_Text(0, 0, (uint8_t *) temp_char_ptr, White, Black);
	
	
	
	for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
        if(!num) matrix_rand[i][j] = tetramino_I[i][j];
				else if(num==1) matrix_rand[i][j] = tetramino_J[i][j];
				else if(num==2) matrix_rand[i][j] = tetramino_L[i][j];
				else if(num==3) matrix_rand[i][j] = tetramino_O[i][j];
				else if(num==4) matrix_rand[i][j] = tetramino_S[i][j];
				else if(num==5) matrix_rand[i][j] = tetramino_T[i][j];
				else if(num==6) matrix_rand[i][j] = tetramino_Z[i][j];
				else GUI_Text(0, 0, (uint8_t *) "funct random_tetramino(...): ERROR", Red, Black);
    }
	}

}

