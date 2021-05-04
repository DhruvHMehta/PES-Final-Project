/*
 * Game.h
 *
 *  Created on: May 2, 2021
 *      Author: Dhruv
 */

#ifndef GAME_H_
#define GAME_H_

/* Function Prototypes */

/**
 * @name    Set_Result.
 *
 * @brief   Function provided for the Command Processing
 * 			module to report completion of a level and
 * 			set the result flag to change Game state.
 *
 * @param	void
 *
 * @return  void
 */
void Set_Result();

/**
 * @name    RunGame.
 *
 * @brief   A state machine which starts and runs the game
 * 			on a terminal emulator program.
 *
 * @param	void
 *
 * @return  void
 */
void RunGame();

#endif /* GAME_H_ */
