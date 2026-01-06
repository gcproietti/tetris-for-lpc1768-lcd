#include "tetris.h"

#ifndef i
uint16_t i = 0;
#endif

#ifndef j
uint16_t j = 0;
#endif


Tetromino tetramino_I = {
    .shape = {
        {{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}}
    },
    .color = Cyan,
    .depth_of_view = 0
};

Tetromino tetramino_O = {
    .shape = {
        {{0, 0, 0, 0},{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0}}
    },
    .color = Yellow,
    .depth_of_view = 0
};

Tetromino tetramino_T = {
    .shape = {
        {{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 0, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 0, 0},{1, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}
    },
    .color = Blue,
    .depth_of_view = 0
};

Tetromino tetramino_L = {
    .shape = {
        {{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 1, 0},{1, 0, 0, 0},{0, 0, 0, 0}},
        {{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 0, 1, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
    },
    .color = Magenta,
    .depth_of_view = 0
};

Tetromino tetramino_J = {
    .shape = {
        {{0, 0, 1, 0},{0, 0, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{1, 0, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 1, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 1, 0},{0, 0, 1, 0},{0, 0, 0, 0}}
    },
    .color = Blue,
    .depth_of_view = 0
};

Tetromino tetramino_S = {
    .shape = {
        {{0, 0, 0, 0},{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 0, 0},{0, 1, 1, 0},{0, 0, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 1, 0, 0},{0, 1, 1, 0},{0, 0, 1, 0},{0, 0, 0, 0}}
    },
    .color = Green,
    .depth_of_view = 0
};

Tetromino tetramino_Z = {
    .shape = {
        {{0, 0, 0, 0},{1, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 1, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
        {{0, 0, 0, 0},{1, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
        {{0, 0, 1, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}}
    },
    .color = Red,
    .depth_of_view = 0
};

//inital coordinates
Coord_str coord_init;

uint16_t score = 0;
uint16_t topScore = 0;
uint16_t lines = 0;

uint8_t field_matrix[10][20];

Tetromino current_tetromino;

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
	
	coord_init.x = 3;				// Non devono essere x = 3 e y = 0?
	coord_init.y = 0;
	
	// initialization of field matrix with all of the elements at zero
	for(i = 0; i < 10; i++){
		for(j = 0; j < 20; j++){
				field_matrix[i][j] = 0;
		}
	}
	
	generate_random_tetraminoes(&current_tetromino);

}

void rotate_tetramino(Tetromino* tetramino){
    tetramino->depth_of_view = (tetramino->depth_of_view + 1) % 4;    
}

/* Funzione che serve per controllare se ci sono delle collisioni
			- Tetromino tetramino: la struct che contiene tutte le rotazioni e l'indice attuale
			- int next_x_pixel: posizione orizzontale in PIXEL
			- int next_y_pixel: posizione verticale in PIXEL
*/
uint8_t check_collision(Tetromino t, int next_x_pixel, int next_y_pixel) {
    int r, c;           // Indici locali tetramino (0-3)
    int row, col;       // Indici globali campo (0-19, 0-9)
    
    // 1. Conversione da pixel a griglia (Grid Coordinate)
    // Formula per arrotondamento per eccesso (ceiling): (val + dim - 1) / dim
    int grid_x = (next_x_pixel + dim - 1) / dim; 
    int grid_y = (next_y_pixel + dim - 1) / dim;

    // 2. Ciclo sui 4x4 blocchi del pezzo
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            
            // --- QUI CAMBIA TUTTO ---
            // Accediamo alla forma corretta usando l'indice di rotazione (depth_of_view)
            // t.shape[ROTAZIONE][RIGA][COLONNA]
            if (t.shape[t.depth_of_view][r][c] == 1) {
                
                // Calcolo posizione assoluta nel campo
                row = grid_y + r;
                col = grid_x + c;

                // A. Controllo Bordi (Muri e Pavimento)
                if (col < 0 || col > 10 || row > 20) {
                    return 1; // Collisione Bordo
                }

                // B. Controllo Sovrapposizione con Blocchi Esistenti
                if (row >= 0) {
                    int valore_pezzo = 1; 
                    // Nota: Qui assumiamo field_matrix[10][20] cioè [COL][ROW] come da tua indicazione
                    int valore_campo = field_matrix[col][row]; 
                    
                    if (valore_pezzo + valore_campo >= 2) {
                        return 1; // Collisione con altro pezzo
                    }
                }
            }
        }
    }

    return 0; // Tutto libero
}
		

Coord_str coordinate_su_schermo(Coord_str coord_matrice){
    Coord_str coord_schermo;
    int offset_x = 3;   // Bordo sinistro
    int offset_y = 20;  // Barra superiore
    
    coord_schermo.x = offset_x + (coord_matrice.x * dim);
    coord_schermo.y = offset_y + (coord_matrice.y * dim);
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
void LCD_tetraminoes(Tetromino tetramino, Coord_str xy, int mode){
	
	int i = 0;
	int l = 0;
	int h = 0;
	uint16_t x0 = xy.x;
	uint16_t y0 = xy.y;
	uint16_t color = tetramino.color;
	if(!mode){color = Black;};
	
  const uint8_t (*matrice)[4] = tetramino.shape[tetramino.depth_of_view];  
	//uint8_t matrice[4][4] = tetramino.shape[tetramino.depth_of_view];
	

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
void update_score(uint16_t score_increment, uint16_t topScore_increment, uint16_t lines_increment){
	
	uint8_t score_char_ptr[6];
	uint8_t topScore_char_ptr[6];
	uint8_t lines_char_ptr[6];
	
	score += score_increment;
	topScore += topScore_increment;
	lines += lines_increment;
	
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

void update_field_matrix(Tetromino t, int current_x_pixel, int current_y_pixel){
    int r, c;
    int row, col;
		uint8_t check_lines = 0;
	
    
    // Convertiamo pixel in indici griglia
    int grid_x = (current_x_pixel + dim - 1) / dim;
    int grid_y = (current_y_pixel + dim - 1) / dim;

    for(r = 0; r < 4; r++){
        for(c = 0; c < 4; c++){
            // Se nel pezzo c'è un blocco pieno
            if(t.shape[t.depth_of_view][r][c] == 1){
                
                row = grid_y + r;
                col = grid_x + c;

                // Controllo sicurezza bordi
                if(row >= 0 && row < 20 && col >= 0 && col < 10){
                    
                    // IMPORTANTE: Matrice definita come [10][20] -> [COL][ROW]
                    field_matrix[col][row] = 1;
                    
                }//else raggiunto in bordo fermare il blocco

            }
        }
    }

}

/******************************************************************************
* Function Name  : lfsr_random
* Description    : Genera un numero pseudo-casuale
*******************************************************************************/
uint8_t lfsr_random(void) {
    static uint8_t state = 0xAC;    // initial seed
    uint8_t remainder = 0x0;
    uint8_t bit;

    if (state == 0) {
        state = 0xAC; 
    }
    
    // Tap bits: 7, 5, 4, 3
    bit  = ((state >> 7) ^ (state >> 5) ^ (state >> 4) ^ (state >> 3)) & 1;
    state = (state >> 1) | (bit << 7);
    remainder = state % 7;

    return remainder;
}

/******************************************************************************
* Function Name  : generate_random_tetraminoes
* Description    : Usa lfsr_random per creare un pezzo e un colore casuale
*******************************************************************************/
void generate_random_tetraminoes(Tetromino* tetramino) {
    // 1. Usa la tua funzione per scegliere la forma (0-6)
    uint8_t num = lfsr_random(); 
		
	uint8_t lines_char_ptr[6];
	uint16_to_ascii_uint8(num, lines_char_ptr);
	GUI_Text(0, 0, (uint8_t *) lines_char_ptr, White, Black);
    
    switch(num) {
        case 0: *tetramino = tetramino_I; break;
        case 1: *tetramino = tetramino_J; break;
        case 2: *tetramino = tetramino_L; break;
        case 3: *tetramino = tetramino_O; break;
        case 4: *tetramino = tetramino_S; break;
        case 5: *tetramino = tetramino_T; break;
        case 6: *tetramino = tetramino_Z; break;
        default: *tetramino = tetramino_I; break; // Fallback di sicurezza
    }

    // Reset rotazione (fondamentale quando nasce un nuovo pezzo)
    tetramino->depth_of_view = 0;

    // 2. Usa di nuovo la tua funzione per scegliere un colore a caso
    uint8_t color_selection = lfsr_random();
    
    switch (color_selection) {
        case 0: tetramino->color = Grey; break;
        case 1: tetramino->color = Blue;  break;
        case 2: tetramino->color = Red; break;
        case 3: tetramino->color = Magenta; break; 
        case 4: tetramino->color = Green; break;
        case 5: tetramino->color = Cyan; break;
        case 6: tetramino->color = Yellow; break; 
        default: break; // Tieni colore originale se esce altro
			
			
    }

}


uint8_t check_cleared_lines(uint8_t field_matrix[10][20]){
	uint8_t check_line = 0;
	uint8_t num_cleared_lines = 0;
	
	for (i = 0; i < 20; i++) {//row
		for (j = 0; j < 10; j++) {//col
			if(field_matrix[j][i]) check_line++;
		}
		if(check_line==10) num_cleared_lines++;
	}
	
	if(num_cleared_lines==4){
		update_score(600,0,num_cleared_lines);
	}else{
		update_score((100*num_cleared_lines),0,num_cleared_lines);
	}
	
	return num_cleared_lines;
				
}