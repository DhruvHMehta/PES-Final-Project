/*
 * Cmd_Processor.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Dhruv
 *      Brief : File contains handlers which process the given
 *      		command and takes action based on the command
 *      		as per the command table handlers.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Cmd_Processor.h"
#include "Game.h"

/* Macros and Type definitions */
#define ROW	0
#define COL	1
#define CUR_ROW_INIT 8
#define CUR_COL_INIT 0
#define CUR_MAX_LEFT 0
#define CUR_MAX_RGHT 38
#define CUR_MAX_UP	 8
#define CUR_MAX_DOWN 18
#define CUR_ROW_WIN	 14
#define CUR_COL_WIN  38
#define	OBS_MAP_COL	 39

/* Function pointer type */
typedef void (*command_handler_t)(int argc, char *argv[]);

typedef struct{
const char *name;
command_handler_t handler;
} command_table_t;

/* Game movement directions */
typedef enum Direction{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
}Direction;

/* Game cursor position and level tracking */
uint8_t cursor_pos[] = {CUR_ROW_INIT, CUR_COL_INIT};
static uint8_t level = 1;

/* Map of obstacles in game
 * single matrix for each level
 * 0 = no object
 * 1 = object present
 */
static const bool object_map[2][429] =
{
	{	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,
		0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},

	{	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,
		1,1,1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,
		0,0,1,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,
		0,0,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,
		0,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,1,
		0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,0,0,
		0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
		0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,1,0,
		0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0}
};

/* Private Functions */

/* Handler for first level */
static void handle_level_1(int argc, char* argv[]);
/* Handler for second level */
static void handle_level_2(int argc, char* argv[]);
/* Handler for up movement command */
static void handle_up(int argc, char* argv[]);
/* Handler for down movement command */
static void handle_down(int argc, char* argv[]);
/* Handler for left movement command */
static void handle_left(int argc, char* argv[]);
/* Handler for right movement command */
static void handle_right(int argc, char* argv[]);
/* Handler for cursor position command */
static void handle_cursor(int argc, char* argv[]);
/* Handler for result print command */
static void handle_result(int argc, char* argv[]);
/* To detect if the cursor will bump into an object */
static uint8_t bump_detect(Direction dir);


/* Add commands here
 * Format : {"Command Name", &handler_fn}
 * */
static const command_table_t commands[] = \
		{

			{"Level_1", &handle_level_1},

			{"Level_2", &handle_level_2},

			{"Up",	   	&handle_up},

			{"Down",   	&handle_down},

			{"Left",	&handle_left},

			{"Right",	&handle_right},

			{"Cursor",  &handle_cursor},

			{"Result",	&handle_result}

		};

static const int num_commands =  sizeof(commands) / sizeof(command_table_t);

/* Function Definitions */

void ParseCommand(char* input)
{
	  char *p = input;
	  char *end;

	  if(p == NULL)
		  return;

	  // find end of string
	  for (end = input; *end != '\0'; end++)
	    ;

	  /* Tokenize input in place */

	  char *argv[2];
	  uint8_t valid_cmd = 0;
	  int argc = 0;
	  memset(argv, 0, sizeof(argv));

	  /* Seperate input into arguments */
	  for (p = input; p < end; p++)
	  {
	    if(*p > 0x20)
	    {
	      argv[argc] = p;
	      argc++;

	      while(*p > 0x20)
	      {
	        p++;
	      }
	    }
	  }

	  argv[argc] = NULL;
	  if (argc == 0)
	  {
		  // no command
		  return;
	  }


	  /* Dispatch command and arguments to correct handler */
	  for (int i=0; i < num_commands; i++)
	  {
		  if (strcasecmp(argv[0], commands[i].name) == 0)
		  {
			  commands[i].handler(argc, argv);
			  valid_cmd = 1;
			  break;
		  }
	  }

	  /* Unknown command handling */
	  if(!valid_cmd)
		  printf("Unknown command : %s\n\r", argv[0]);

}


static uint8_t bump_detect(Direction dir)
{
	/* Checks the direction in which the cursor intends to move
	 * and checks if an obstacle is present in that direction
	 * Return 0 if obstacle is present to prevent collision.
	 *
	 * General calculation formula :
	 * Check object map 2-D array for the current level and cursor position
	 * Based on the direction intended, calculate the position of the
	 * cursor mapped to the object map.
	 */

	if(dir == UP)
	{
		if(object_map[level-1][(cursor_pos[ROW] - CUR_ROW_INIT - 1)*OBS_MAP_COL + (cursor_pos[COL])])
			return 0;
	}

	else if(dir == DOWN)
	{
		if(object_map[level-1][(cursor_pos[ROW] - CUR_ROW_INIT + 1)*OBS_MAP_COL + (cursor_pos[COL])])
			return 0;
	}

	else if(dir == LEFT)
	{
		if(object_map[level-1][(cursor_pos[ROW] - CUR_ROW_INIT)*OBS_MAP_COL + (cursor_pos[COL] - 1)])
			return 0;
	}

	else if(dir == RIGHT)
	{
		if(object_map[level-1][(cursor_pos[ROW] - CUR_ROW_INIT)*OBS_MAP_COL + (cursor_pos[COL] + 1)])
			return 0;
	}

	return 1;
}

static void handle_level_1(int argc, char* argv[])
{
	/* Color blue, scroll to fill screen, resize window
	 * move cursor to 1,1 */
	printf("\e[44m\n\r\e[2J\n\r\e[8;24;80t\n\r\e[1;1H");

	/* Print out the UI, maze and instruction. Move cursor to the start and stop cursor blink */
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("                         __   __   ___  _                                       \n\r");
	printf("               |\\/|  /\\   /  |_     |  |_ |) |\\/| | |\\|  /\\  |             \n\r");
	printf("               |  | /  \\ /_  |__    |  |_ |\\ |  | | | | /  \\ |__             \n\r");
	printf("                                                                                \n\r");
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("_______________________________________                                         \n\r");
	printf("                                       |                                        \n\r");
	printf("                  _       |       _____|    Welcome to Maze Terminal!           \n\r");
	printf("  |____________    |      |      |     |    Use your NXP Dev Board as           \n\r");
	printf("    |              |______|  __________|    a controller, USB facing            \n\r");
	printf("____|  |  ______                       |    the user.                           \n\r");
	printf("       |_|  |   |_________________|____|                                        \n\r");
	printf("    |       |               |               Hold the board horizontally and     \n\r");
	printf("    |_______|        ___    |          |    tap the touch sensor to start       \n\r");
	printf("            |_____  |                  |                                        \n\r");
	printf("  |_________|       |   |____________  |    Pitch up/down to move up/down       \n\r");
	printf("               |    |                | |    Roll left/right to move left/right  \n\r");
	printf("_______________|____|________________|_|    To win -> Guide * to opening of maze\n\r\e[8;1H\e[?25l");
}

static void handle_level_2(int argc, char* argv[])
{
	/* Color red, scroll to fill screen,
	 * move cursor to 1,1 */
	printf("\e[41m\n\r\e[2J\n\r\e[1;1H");

	/* Print out the UI, maze and instruction. Move cursor to the start and stop cursor blink */
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("                         __   __   ___  _                                       \n\r");
	printf("               |\\/|  /\\   /  |_     |  |_ |) |\\/| | |\\|  /\\  |             \n\r");
	printf("               |  | /  \\ /_  |__    |  |_ |\\ |  | | | | /  \\ |__             \n\r");
	printf("                                                                                \n\r");
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("_______________________________________                                         \n\r");
	printf("                                       |                                        \n\r");
	printf("      ______|==  ===========   ==  === |     Welcome to Maze Terminal!          \n\r");
	printf("____ |        |       ______| |  | | | |                                        \n\r");
	printf("  |    |====  |  ____|   ___  |__      |             Level 2!                   \n\r");
	printf("  | _  |    __  |       |   |    |  ___|                                        \n\r");
	printf("     | |__ |  |_| ______|   |    |_  __|                                        \n\r");
	printf("  |  |         __           |___ |  |                                           \n\r");
	printf("  |__|  ______|  |______|____________| |                                        \n\r");
	printf("  |    |      |_        |  ______      |                                        \n\r");
	printf("  | ___|         | _____| |       ___| |                                        \n\r");
	printf("  |      |_______|              | |    |                                        \n\r");
	printf("____|_|__|__|__|__|__|__|_|______|_____|                                        \n\r\e[8;1H\e[?25l");

	level = 2;
}

static void handle_up(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[ROW] != CUR_MAX_UP && bump_detect(UP))
	{
		/* Erase old position, move up, write * */
		printf("\b \b\e[A*");
		cursor_pos[ROW] -= 1;
	}

}


static void handle_down(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[ROW] != CUR_MAX_DOWN && bump_detect(DOWN))
	{
		/* Erase old position, move down, write * */
		printf("\b \b\eD*");
		cursor_pos[ROW] += 1;
	}

}


static void handle_left(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[COL] != CUR_MAX_LEFT && bump_detect(LEFT))
	{
		/* Erase old position, move left, write * */
		printf("\b \b\e6*");
		cursor_pos[COL] -= 1;
	}

}


static void handle_right(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[COL] != CUR_MAX_RGHT && bump_detect(RIGHT))
	{
		/* Erase old position, move up, write * */
		printf("\b \b\e9*");
		cursor_pos[COL] += 1;
	}
}

static void handle_cursor(int argc, char* argv[])
{
	/* Check if the maze is completed */
	if((cursor_pos[ROW] == CUR_ROW_WIN) && (cursor_pos[COL] == CUR_COL_WIN))
		Set_Result();
}

static void handle_result(int argc, char* argv[])
{
	printf("\e[21;30H Level Complete!");

	if(level == 1)
    printf("\e[22;28H Tap to go to Level 2");

	else
	{
		/* Hang here, game has ended */
		printf("\e[22;18H You win! Please press the reset button to start over");
		while(1);
	}

	/* Reset cursor for next level */
	cursor_pos[ROW] = CUR_ROW_INIT;
	cursor_pos[COL] = CUR_COL_INIT;
}


