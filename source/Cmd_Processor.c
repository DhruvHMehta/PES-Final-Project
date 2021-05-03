/*
 * Cmd_Processor.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Dhruv
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Cmd_Processor.h"
#include "Game.h"

/* Macros and Type definitions */
#define MAX_DUMP 640
#define MIN(x,y) (x < y ? (x) : (y))

#define ROW	0
#define COL	1

/* Function pointer type */
typedef void (*command_handler_t)(int argc, char *argv[]);

typedef struct{
const char *name;
command_handler_t handler;
const char *help_string;
} command_table_t;

uint8_t cursor_pos[] = {8,0};
static uint8_t level = 1;

typedef enum Direction{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
}Direction;

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
void ParseCommand(char* input);
static void handle_level_1(int argc, char* argv[]);
static void handle_level_2(int argc, char* argv[]);
static void handle_up(int argc, char* argv[]);
static void handle_down(int argc, char* argv[]);
static void handle_left(int argc, char* argv[]);
static void handle_right(int argc, char* argv[]);
static void handle_cursor(int argc, char* argv[]);
static void handle_result(int argc, char* argv[]);


/* Add commands here
 * Format : {"Command Name", &handler_fn, "Help string"}
 * */
static const command_table_t commands[] = \
		{

			{"Level_1", &handle_level_1, " ddd"},

			{"Level_2", &handle_level_2, "aad"},

			{"Up",	   	&handle_up, "dd"},

			{"Down",   	&handle_down, "ee"},

			{"Left",	&handle_left, "dd"},

			{"Right",	&handle_right, "dd"},

			{"Cursor",  &handle_cursor, "dd"},

			{"Result",	&handle_result, "aa"}

		};

static const int num_commands =  sizeof(commands) / sizeof(command_table_t);

/* Function Definitions */

/**
 * @name    Parse_Command.
 *
 * @brief   Tokenizes the input string and calls the
 * 			correct handler for the command.
 *
 * @param	char* input - Accumulated buffer string.
 *
 * @return  void
 *
 */
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
	if(dir == UP)
	{
		if(object_map[level-1][(cursor_pos[ROW] - 9)*39 + (cursor_pos[COL])])
			return 0;
	}

	else if(dir == DOWN)
	{
		if(object_map[level-1][(cursor_pos[ROW] - 7)*39 + (cursor_pos[COL])])
			return 0;
	}

	else if(dir == LEFT)
	{
		if(object_map[level-1][(cursor_pos[ROW] - 8)*39 + (cursor_pos[COL] - 1)])
			return 0;
	}

	else if(dir == RIGHT)
	{
		if(object_map[level-1][(cursor_pos[ROW] - 8)*39 + (cursor_pos[COL] + 1)])
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
	printf("                  _       |       _____|     Welcome to Maze Terminal!          \n\r");
	printf("  |____________    |      |      |     |     Use your NXP Dev Board as          \n\r");
	printf("    |              |______|  __________|     a controller, USB facing           \n\r");
	printf("____|  |  ______                       |     the user.                          \n\r");
	printf("       |_|  |   |_________________|____|                                        \n\r");
	printf("    |       |               |                Hold the board horizontally and    \n\r");
	printf("    |_______|        ___    |          |     tap the touch sensor to start      \n\r");
	printf("            |_____  |                  |                                        \n\r");
	printf("  |_________|       |   |____________  |     Pitch up/down to move up/down      \n\r");
	printf("               |    |                | |     Roll left/right to move left/right \n\r");
	printf("_______________|____|________________|_|     To win -> Reach opening of maze    \n\r\e[8;1H\e[?25l");
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
	if(cursor_pos[ROW] != 8 && bump_detect(UP))
	{
		/* Erase old position, move up, write * */
		printf("\b \b\e[A*");
		cursor_pos[ROW] -= 1;
	}

}


static void handle_down(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[ROW] != 18 && bump_detect(DOWN))
	{
		/* Erase old position, move down, write * */
		printf("\b \b\eD*");
		cursor_pos[ROW] += 1;
	}

}


static void handle_left(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[COL] != 0 && bump_detect(LEFT))
	{
		/* Erase old position, move left, write * */
		printf("\b \b\e6*");
		cursor_pos[COL] -= 1;
	}

}


static void handle_right(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if(cursor_pos[COL] != 38 && bump_detect(RIGHT))
	{
		/* Erase old position, move up, write * */
		printf("\b \b\e9*");
		cursor_pos[COL] += 1;
	}
}

static void handle_cursor(int argc, char* argv[])
{
	/* Check for boundary walls and obstacles */
	if((cursor_pos[ROW] == 14) && (cursor_pos[COL] == 38))
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

	cursor_pos[ROW] = 8;
	cursor_pos[COL] = 0;
}


