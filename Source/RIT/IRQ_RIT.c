/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Descriptions:        Functions to manage T0 and T1 interrupts
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "tetris/tetris.h"

/******************************************************************************
** Function name:       RIT_IRQHandler
** Descriptions:        REPETITIVE INTERRUPT TIMER handler
******************************************************************************/

#define SPEED_LIMIT 10
#define FAST_SPEED 2
extern volatile int k1down;
extern volatile int k2down;             
void GUI_gameover_view();
// --- EXTERNAL VARIABLES (Defined in tetris.c or main.c) ---
extern Tetromino current_tetromino;    // Structure of the current piece
extern Coord_str coord_init;           // LOGICAL coordinates (Grid: x=0..9, y=0..19)
extern uint16_t field_matrix[10][20];   // Game board matrix
extern uint16_t score;
extern uint16_t topScore;
extern uint16_t lines;
extern volatile int hard_drop_requested; // 1 quando l'utente preme il tasto
extern volatile uint8_t state_game;           // 1 = play, 0 = pause
extern volatile uint8_t gravity_tick;
extern volatile uint8_t rotation_requested;
extern volatile uint8_t gameover_flag;
extern volatile uint8_t move_right_requested;
extern volatile uint8_t move_left_requested;
extern volatile uint8_t reset_requested;

void RIT_IRQHandler(void)
{
    static int tick = 0;
    int current_speed_limit = SPEED_LIMIT;      // 20 ticks = 1 second (Standard speed)
        
    // Static variables for joystick debouncing/state
    static int up = 0;                          
    static int right = 0;
    static int left = 0;
    //GUI_Text(160, 220, (uint8_t *) "PAUSE__", Red, Black);

    // ---------------------------------------------------
    // 1. KEY1 MANAGEMENT (PAUSE / RESUME)
    // ---------------------------------------------------
if (k1down >= 1) {
    if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0) { // Button pressed (Active Low)
        switch (k1down) {
            case 1: 
                if (!state_game && !gameover_flag) {
                    state_game = 1;
										//GUI_Text(160, 240, (uint8_t *) "PRESS", Black, Black);
										//GUI_Text(160, 260, (uint8_t *) "KEY1", Black, Black);
										//GUI_Text(160, 280, (uint8_t *) "TO START", Black, Black);
                    GUI_Text(160, 220, (uint8_t *) "INGAME", Green, Black);
                } else if (state_game) {
                    state_game = 0;
                    GUI_Text(160, 220, (uint8_t *) "PAUSE ", Yellow, Black);
                } else if (gameover_flag) {
										reset_requested = 1;
								}
                break;
            default: 
                break;
        }
        k1down++;
    }
    else { // Button released
        k1down = 0;
        LPC_PINCON->PINSEL4 |= (1 << 22); // Re-enable EINT1
        NVIC_EnableIRQ(EINT1_IRQn);
    }
}
    
    // ---------------------------------------------------
    // 2. GAME LOGIC (Only active if state_game == 1)
    // ---------------------------------------------------
    if (state_game) {
        
        // --- A. Joystick DOWN (Acceleration / Soft Drop) ---
        if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
            current_speed_limit = FAST_SPEED; // Fast speed (0.1s)
        }
        else {
            current_speed_limit = SPEED_LIMIT; // Normal speed (1s) - Fixed from FAST_SPEED to SPEED_LIMIT based on init var
        }
            
        // --- B. Gravity Timer (Automatic Descent) ---
        tick++;
        if (tick >= current_speed_limit) { 
            tick = 0;
            gravity_tick = 1; // Notifica il main
					
        }
            
        // --- C. Joystick UP (Rotation) ---
        if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
            up++;
            switch (up) {
                case 1:
                    {
											rotation_requested = 1;
                    }
                    break;
                default: 
                    break;
            }
        }
        else { 
            up = 0; 
        }
            
        // --- D. Joystick RIGHT ---
        if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
            right++;
            switch (right) {
                case 1:
										move_right_requested = 1; // Segnala al main
                    
                    break;
                default: 
                    break;
            }
        }
        else { 
            right = 0; 
        }
            
        // --- E. Joystick LEFT ---
        if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
            left++;
            switch (left) {
                case 1:
                    move_left_requested = 1; // Segnala al main
                    break;
                default: 
                    break;
            }
        }
        else { 
            left = 0; 
        }

        // ---------------------------------------------------
        // 3. KEY2 (HARD DROP - Instant Fall)
        // ---------------------------------------------------
        if (k2down >= 1) {
            // Check if the button is physically pressed (Active Low check on Bit 12)
            if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0) { 
                
                switch (k2down) {
                    case 1: { // Executed on first detection
													hard_drop_requested = 1; // Segnala al gioco di eseguire l'hard drop
													k2down++;                // Blocca ripetizioni
													break;
                    }

                    default: 
                        break;
                }
                
                // Increment counter to block repeated execution while holding the button
                k2down++;
            } 
            else { 
                // Button Released
                k2down = 0;

                // Re-enable External Interrupt (EINT2) functionality on P2.12
                LPC_PINCON->PINSEL4 |= (1 << 24); 
                NVIC_EnableIRQ(EINT2_IRQn);
            }
        }
    } // End of state_game == 1
    
    LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}

