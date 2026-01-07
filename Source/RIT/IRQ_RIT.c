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
volatile int state_game = 0;           // 1 = play, 0 = pause
void GUI_gameover_view();
// --- EXTERNAL VARIABLES (Defined in tetris.c or main.c) ---
extern Tetromino current_tetromino;    // Structure of the current piece
extern Coord_str coord_init;           // LOGICAL coordinates (Grid: x=0..9, y=0..19)
extern uint8_t field_matrix[10][20];   // Game board matrix
extern uint16_t score;
extern uint16_t topScore;
extern uint16_t lines;

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
                if (state_game == 0) {
                    state_game = 1;
                    GUI_Text(160, 220, (uint8_t *) "INGAME", Green, Black);
                } else {
                    state_game = 0;
                    GUI_Text(160, 220, (uint8_t *) "PAUSE ", Yellow, Black);
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
    if (state_game == 1) {
        
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
            
            // Check Collision: Convert grid coordinates to pixels (* dim)
            // Check future position: y + 1
            if (check_collision(current_tetromino, coord_init.x, coord_init.y + 1) == 0) {
                
                // 1. Erase old position
                LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                
                // 2. Update Logic (Grid)
                coord_init.y++; // Move down by 1 block
                
                // 3. Draw new position
                LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
            }
            else {
                // HIT THE BOTTOM OR ANOTHER PIECE
                update_field_matrix(current_tetromino, coord_init.x, coord_init.y);
								update_score(10,0,0);
								check_and_clear_lines();
								
                // Reset coordinates to top spawn position
                coord_init.x = 3; 
                coord_init.y = 0;
                
                // Generate a new piece
                generate_random_tetraminoes(&current_tetromino);
                
                // Check for GAME OVER (Collision immediately after spawn)
                if (check_collision(current_tetromino, coord_init.x, coord_init.y + 1) == 1) { 
									GUI_gameover_view();
                    state_game = 0; // Stop the game loop
                }
            }
        }
            
        // --- C. Joystick UP (Rotation) ---
        if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
            up++;
            switch (up) {
                case 1:
                    {
                        // 1. Create a temporary copy
                        Tetromino temp = current_tetromino;
                        
                        // 2. Rotate the copy
                        rotate_tetramino(&temp);
                        
                        // 3. Check if the rotated copy fits
                        if (check_collision(temp, coord_init.x, coord_init.y) == 0) {
                            
                            // Erase old (current shape)
                            LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                            
                            // Update official struct with the rotated one
                            current_tetromino = temp; 
                            
                            // Draw new (rotated shape)
                            LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                        }
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
                    // Check future position: x + 1
                    if (check_collision(current_tetromino, coord_init.x + 1, coord_init.y) == 0) {
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                        coord_init.x++; // Move 1 block right
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                    }
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
                    // Check future position: x - 1
                    if (check_collision(current_tetromino, coord_init.x - 1, coord_init.y) == 0) {
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                        coord_init.x--; // Move 1 block left
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);
                    }
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
                        int max_drops = 20;  // Safety limit to prevent infinite loops

                        // 1. Erase the tetromino from its current position (Color 0 = background)
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 0);
                        
                        // 2. Calculate the landing position (simulate gravity loop)
                        // Check collision for the next row down; if free, move y down.
                        while (check_collision(current_tetromino, coord_init.x, coord_init.y + 1) == 0 
                               && max_drops-- > 0) {
                            coord_init.y++;
                        }

                        // 3. Draw the tetromino at the final impact position (Color 1 = foreground)
                        LCD_tetraminoes(current_tetromino, coordinate_su_schermo(coord_init), 1);

                        // 4. Lock the piece into the game grid matrix
                        update_field_matrix(current_tetromino, coord_init.x, coord_init.y);
												
												// Gestione punteggio
												score += 10;
												if (score > topScore) topScore = score;
												update_score(score, topScore, lines);
												
												check_and_clear_lines();
												
                        // 5. Reset spawn coordinates for the new piece
                        coord_init.x = 3;
                        coord_init.y = 0;

                        // 6. Generate the next piece
                        generate_random_tetraminoes(&current_tetromino);
                        
                        // 7. Check Game Over immediately after Hard Drop
                         if (check_collision(current_tetromino, coord_init.x, coord_init.y + 1) == 1) { 
                            GUI_Text(70, 60, (uint8_t *) "GAME OVER!!", Red, Black);
														if(score>topScore) update_score(0,score-topScore,0);
                            state_game = 0;
                        }
                        
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

void GUI_gameover_view(void) {
    // ---------------------------------------------------
    // Configuration
    // ---------------------------------------------------
    // Box dimensions and position (Centered for 240px width)
    int x_start = 30;
    int y_start = 130;
    int width = 180;
    int height = 60;
    int i, j, idx;
    // A. Draw Shadow (Offset by 3px for depth)
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            LCD_SetPoint(x_start + i + 3, y_start + j + 3, Black);
        }
    }

    // B. Draw Main Box Background (Red) and Border (White)
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            if (i < 2 || i > width - 3 || j < 2 || j > height - 3) {
                // Draw Border
                LCD_SetPoint(x_start + i, y_start + j, White);
            } else {
                // Draw Body
                LCD_SetPoint(x_start + i, y_start + j, Red);
            }
        }
    }


    GUI_Text(x_start + 45, y_start + 22, (uint8_t *)"GAME OVER", White, Red);

    // Holds the screen for approx 2-3 seconds so the user sees the message
    volatile int delay_counter;
    for (delay_counter = 0; delay_counter < 10000000; delay_counter++);
}

