# PES-Final-Project
 A Maze game which runs on a terminal emulator, controlled by an accelerometer.

## Table of Contents
* [Description](#description)
* [Operation](#operation)
* [Files](#files)
* [Build](#build)
* [References](#references)
* [Author](#author)

## Description
Contains an MCUXpresso Project for a Terminal-based Maze game. Requires the user to use the KL25Z4 on-board accelerometer to guide a cursor across two levels of the game.
To play, simply build the project as described in the build section and open a terminal emulator program ([TeraTerm](https://ttssh2.osdn.jp/index.html.en) works, the in-built terminal will not). Instructions are displayed in the game. 
Please refer to the [Extra](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/Extra) section for the testing strategy and screenshots.

*Serial Settings - 38400 baud, 8 data bits, 2 stop bits, no parity.

## Operation
Uses UART to print a maze game on the terminal along with [Control Sequences](https://ttssh2.osdn.jp/manual/4/en/about/ctrlseq.html) & [ANSI Escape Codes](https://en.wikipedia.org/wiki/ANSI_escape_code) to control the movement of the cursor on the screen using roll and pitch angles
obtained from the accelerometer.

### Files
* [MazeTerminal.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/MazeTerminal.c) - Contains the application entry point which runs the initialization and starts the game.
* [Game.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/Game.c) - Contains the state machine of the game which switches between different modes.
* [UART.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/UART.c) - Contains the UART initialization and Interrupt Handler. 
* [I2C.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/I2C.c) - Contains the I2C initialization for the Accelerometer. 
* [MMA8451.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/MMA8451.c) - Contains the Initialization, data gathering and conversion functions to pitch and roll angles. 
* [Cmd_Processor.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/Cmd_Processor.c) - Contains the Command Processor and its supporting functions. Used internally.
* [cbfifo.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/cbfifo.c) - Contains a circular buffer based FIFO implementation.
* [Testsuite.c](https://github.com/DhruvHMehta/PES-Final-Project/blob/main/source/Testsuite.c) - Contains the uCUnit based test framework and test cases. 

Several other supporting files exist which are not mentioned.

## Build
MCUXpresso IDE v11.3.0 [Build 5222] [2021-1-11]

Build the Project (Hammer Button / Ctrl + B) in Debug Mode
Connect Board via USB Cable
Dump to Board (Blue Bug)

Test results and timing data can be viewed on the terminal via UART.

## References
* Professor Howdy Pierce
* Professor Alexander G Dean - Embedded Systems Fundamentals with ARM Cortex-M based Microcontrollers: A Practical Approach
* KL25Z Reference Manual
* MMA8451Q Technical Datasheet

## Author
* Dhruv Mehta
