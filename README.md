# Tetris for LandTiger (LPC1768)

![Platform](https://img.shields.io/badge/Platform-NXP_LPC1768-blue)
![Environment](https://img.shields.io/badge/IDE-Keil_uVision_5-green)
![Language](https://img.shields.io/badge/Language-C_%2F_Assembly-orange)

## 📋 Overview
This project is a bare-metal implementation of the classic **Tetris** game for the **LandTiger Board** (based on NXP LPC1768 Cortex-M3 microcontroller).

The project was developed using **Keil µVision** and demonstrates low-level system programming, including direct management of hardware peripherals, interrupt handling, and graphical rendering on an LCD display.

## ✨ Features
* **Full Gameplay Loop:** Implements standard mechanics including piece rotation, gravity, collision detection, and line clearing.
* **Tetromino System:** Includes all 7 standard shapes (I, O, T, J, L, S, Z) with random generation.
* **Graphics:** Custom rendering engine for the $20 \times 10$ grid and UI elements on the specific LandTiger LCD.
* **Score & Stats:** Real-time tracking of:
  * Current Score
  * High Score (session-persistent)
  * Lines Cleared
* **State Management:** Finite State Machine (FSM) managing Game Over, Pause, and Playing states.

## 🕹️ Controls
The game utilizes the on-board Joystick and Buttons:

| Input | Function |
| :--- | :--- |
| **KEY1** | **Start / Pause / Resume** Game |
| **KEY2** | **Hard Drop** (Instant placement) |
| **Joystick LEFT** | Move Piece Left |
| **Joystick RIGHT** | Move Piece Right |
| **Joystick UP** | Rotate Piece ($90^{\circ}$ Clockwise) |
| **Joystick DOWN** | **Soft Drop** (Increase fall speed) |

## ⚙️ Technical Architecture
The system is built without an Operating System (Bare-Metal), utilizing the following hardware resources:

* **RIT (Repetitive Interrupt Timer):** Handles the game tick, touch/joystick polling, and generic timing.
* **Timer0 / Timer1:** Used for scheduling game events and audio generation (if applicable).
* **GLCD (Graphical LCD):** Direct pixel manipulation for rendering the game field and text.
* **GPIO / EINT:** Interrupt-based handling for buttons (KEY1, KEY2).

## 🚀 How to Run
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/gcproietti/tetris-for-lpc1768-lcd](https://github.com/gcproietti/tetris-for-lpc1768-lcd)
    ```
2.  **Open in Keil µVision:**
    * Launch Keil µVision 5.
    * Open the `.uvprojx` file located in the project root.
3.  **Build & Debug:**
    * Select the target **SW_Debug** (configured for the Emulator).
    * Build the project (`F7`).
    * Start the Debug Session (`Ctrl + F5`).
4.  **Emulation:**
    * Ensure the **LandTiger** peripheral window is open.
    * Run the simulation.

> **Note:** Proper emulator configuration (Timer scaling, RIT settings) is required for correct speed execution. Please refer to the `setup.docx` or project screenshots for the specific Keil settings.
