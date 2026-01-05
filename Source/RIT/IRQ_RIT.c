/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Descriptions:        functions to manage T0 and T1 interrupts
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "tetris/tetris.h"

/******************************************************************************
** Function name:       RIT_IRQHandler
** Descriptions:        REPETITIVE INTERRUPT TIMER handler
******************************************************************************/

extern volatile int k1down;
extern volatile int k2down;             
volatile int state_game = 1;           // 1 = play, 0 = pausa

// --- VARIABILI ESTERNE (Definite in tetris.c o main.c) ---
extern Tetromino current_tetromino;    // La struct del pezzo attuale
extern Coord_str coord_init;           // Coordinate LOGICHE (Griglia: x=0..9, y=0..19)
extern uint8_t field_matrix[10][20];   // Serve se devi resettare o controllare

void RIT_IRQHandler (void)
{
    static int tick = 0;
    int current_speed_limit = 20;      // 20 ticks = 1 secondo (Standard)
        
    // Variabili statiche per i joystick
    static int up = 0;                          
    static int right = 0;
    static int left = 0;
    
    // ---------------------------------------------------
    // 1. GESTIONE KEY1 (PAUSA)
    // ---------------------------------------------------
    if(k1down >= 1){
        if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ // Tasto premuto
            switch(k1down){
                case 2: // Debounce ok
                    if (state_game == 0) state_game = 1;
                    else state_game = 0;
                    break;
                default: break;
            }
            k1down++;
        }
        else{ // Tasto rilasciato
            k1down = 0;
            LPC_PINCON->PINSEL4 |= (1 << 22);
            NVIC_EnableIRQ(EINT1_IRQn);
        }
    }
    
    // ---------------------------------------------------
    // 2. GESTIONE GIOCO (Solo se state_game == 1)
    // ---------------------------------------------------
    if(state_game == 1){
        
        // --- A. Joystick DOWN (Accelerazione) ---
        if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
            current_speed_limit = 2; // Molto veloce (0.1s)
        }
        else {
            current_speed_limit = 3; // Normale (1s)
        }
            
        // --- B. Timer Gravità (Discesa Automatica) ---
        tick++;
        if (tick >= current_speed_limit){ 
            tick = 0;
            
            // Check Collisione: Converto coordinate griglia in pixel (* dim) per il controllo
            // Controllo posizione futura: y + 1
            if (check_collision(current_tetromino, coord_init.x * dim, (coord_init.y + 1) * dim) == 0){
                
                // 1. Cancella vecchio
                LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                
                // 2. Aggiorna Logica (Griglia)
                coord_init.y++; // Scendo di 1 blocco
                
                // 3. Disegna nuovo
                LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
            }
            else {
                // HO TOCCATO IL FONDO
                update_field_matrix(current_tetromino, coord_init.x * dim, coord_init.y * dim);
                
                // Qui dovresti generare un nuovo pezzo.
                // Reset coordinate in cima
                coord_init.x = 3; 
                coord_init.y = 0;
                generate_random_tetraminoes(&current_tetromino);
                
                // Se spawni e sbatti subito -> GAME OVER (da implementare)
            }
        }
            
        // --- C. Joystick UP (Rotazione) ---
        if ((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
            up++;
            switch(up){
                case 1:
                    {
                        // 1. Crea copia temporanea
                        Tetromino temp = current_tetromino;
                        
                        // 2. Ruota la copia
                        rotate_tetramino(&temp);
                        
                        // 3. Controlla se la copia ci sta (passando coordinate pixel)
                        if (check_collision(temp, coord_init.x * dim, coord_init.y * dim) == 0) {
                            
                            // Cancella vecchio (forma attuale)
                            LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                            
                            // Aggiorna struct ufficiale
                            current_tetromino = temp; 
                            
                            // Disegna nuovo (forma ruotata)
                            LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                        }
                    }
                    break;
                default: break;
            }
        }
        else { up = 0; }
            
        // --- D. Joystick RIGHT ---
        if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
            right++;
            switch(right){
                case 1:
                    // Controllo posizione futura: x + 1
                    if(check_collision(current_tetromino, (coord_init.x + 1) * dim, coord_init.y * dim) == 0) {
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                        coord_init.x++; // Sposto di 1 blocco a destra
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                    }
                    break;
                default: break;
            }
        }
        else { right = 0; }
            
        // --- E. Joystick LEFT ---
        if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
            left++;
            switch(left){
                case 1:
                    // Controllo posizione futura: x - 1
                    if(check_collision(current_tetromino, (coord_init.x - 1) * dim, coord_init.y * dim) == 0) {
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                        coord_init.x--; // Sposto di 1 blocco a sinistra
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                    }
                    break;
                default: break;
            }
        }
        else { left = 0; }
            
        // ---------------------------------------------------
        // 3. KEY2 (HARD DROP - Caduta Istantanea)
        // ---------------------------------------------------
        if(k2down >= 1){
            if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ // Premuto
                switch(k2down){
                    case 2:{
															int max_drops = 20;  // Safety limit
															while(check_collision(current_tetromino, coord_init.x * dim, (coord_init.y + 1) * dim) == 0 
																		&& max_drops-- > 0) {
																	LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
																	coord_init.y++;
																	LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
															}
															update_field_matrix(current_tetromino, coord_init.x * dim, coord_init.y * dim);
															coord_init.x = 3;
															coord_init.y = 0;
															generate_random_tetraminoes(&current_tetromino);
													}
													break;
                        
                    default: break;
                }
                k2down++;
            }
            else{ // Rilasciato
                k2down = 0;
                LPC_PINCON->PINSEL4 |= (1 << 24);
                NVIC_EnableIRQ(EINT2_IRQn);
            }
        }
    }
    
    LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}