#ifndef __TETRIS_H
#define __TETRIS_H

#include "LPC17xx.h"
#include "GLCD/GLCD.h"

#define dim 14	//dimension of foundamental block (square 14px * 14px)
// Nota: x_init e y_init potrebbero non servire più se usi coord_init (3,0)
#define x_init 41 
#define y_init 23
#define START_X_POS_TETRAMINO 3
#define START_Y_POS_TETRAMINO 0


#ifndef i
extern uint16_t i;
#endif

#ifndef j
extern uint16_t j;
#endif

// --- DEFINIZIONE STRUTTURE ---

//typedef struct {
//    uint16_t x;
//    uint16_t y;
//} Coord_str;

typedef struct {
    int x;
    int y;
} Coord_str;
	
typedef struct {
    uint8_t shape[4][4][4]; 
    uint16_t color;
    int depth_of_view; 
} Tetromino;

// --- VARIABILI ESTERNE ---

extern Tetromino tetramino_I;
extern Tetromino tetramino_O;
extern Tetromino tetramino_T;
extern Tetromino tetramino_L;
extern Tetromino tetramino_J;
extern Tetromino tetramino_S;
extern Tetromino tetramino_Z;

extern uint16_t score;
extern uint16_t topScore;
extern uint16_t lines;

// Attenzione: field_matrix[10][20] significa accesso [colonna][riga]
extern uint16_t field_matrix[10][20];

// CORREZIONE IMPORTANTE: Ora current_tetromino è una Struct, non una matrice 4x4!
extern Tetromino current_tetromino; 

extern Coord_str coord_init;

extern volatile uint8_t state_game;           // 1 = play, 0 = pause
extern volatile uint8_t gravity_tick;
extern volatile uint8_t rotation_requested;
extern volatile uint8_t gameover_flag;
extern volatile uint8_t move_right_requested;
extern volatile uint8_t move_left_requested;

// --- PROTOTIPI FUNZIONI ---

void tetrisInit();
void generate_random_tetraminoes(Tetromino* tetramino);
uint8_t lfsr_random();
void rotate_tetramino(Tetromino* tetramino);

// Questa è la riga che mancava e causava l'errore "passing int...":
Coord_str coordinate_su_schermo(Coord_str coord_matrice); 

void LCD_tetraminoes(Tetromino tetramino, Coord_str xy, int mode);
void update_score(uint16_t score_increment, uint16_t topScore_increment, uint16_t lines_increment);
void uint16_to_ascii_uint8(uint16_t val, uint8_t *dest);

// Nuovi prototipi aggiornati con Struct Tetromino
void update_field_matrix(Tetromino t, int current_x, int current_y);
uint8_t check_collision(Tetromino t, int next_x, int next_y);
void check_and_clear_lines();
void redraw_field();


void execute_hard_drop_logic();
void execute_rotation_logic();
void handle_gravity();
void GUI_gameover_view();
void handle_right_movement();
void handle_left_movement();

#endif