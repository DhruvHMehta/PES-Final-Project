/*
 * Cmd_Processor.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Dhruv
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Cmd_Processor.h"

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

uint8_t cursor_pos[] = {8,1};

/* Private Functions */
void ParseCommand(char* input);
static void handle_author(int argc, char *argv[]);
static void handle_help(int argc, char* argv[]);
static void handle_test(int argc, char* argv[]);
static void handle_up(int argc, char* argv[]);
static void handle_down(int argc, char* argv[]);
static void handle_left(int argc, char* argv[]);
static void handle_right(int argc, char* argv[]);

/* Add commands here
 * Format : {"Command Name", &handler_fn, "Help string"}
 * */
static const command_table_t commands[] = \
		{
			{"Author",	&handle_author, "Author\n\r\t" \
										"Prints the name of the author\n\r"},

			{"Help",	&handle_help,	"Help\n\r\t" \
										"Prints this help message\n\r"},

			{"Test",   	&handle_test, " ddd"},

			{"Up",	   	&handle_up, "dd"},

			{"Down",   	&handle_down, "ee"},

			{"Left",	&handle_left, "dd"},

			{"Right",	&handle_right, "dd"}
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

	  // find end of string
	  for (end = input; *end != '\0'; end++)
	    ;

	  /* Tokenize input in place */

	  char *argv[10];
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
	      //*p = '\0';
	    }
	  }

	  argv[argc] = NULL;
	  if (argc == 0)
	  {
		  // no command
		  //printf("? ");
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

	  //printf("? ");
}


void LineAccumalator()
{
	static char input[100];
	static uint8_t i = 0;

	/* If any byte is received, add to the buffer */
	if(ByteReceived())
	{
		input[i] = getchar();
		i++;

		/* Backspace handling */
		if(input[i-1] == '\b')
		{
			i = i - 2;

			/* To prevent backspace from eating the prompt */
			if(i == 255)
			{
				printf(" ");
				i++;
			}

			else printf(" \b");
		}

		/* On reception of enter, Process the command */
		if(input[i-1] == '\r')
		{
			input[i] = '\0';
			i++;

			printf("\r\n");
			ParseCommand(input);

			i = 0;
		}
	}
}

/**
 * @name    handle_author.
 *
 * @brief   Handles the command - author, by printing my name.
 *
 * @param	int argc - Number of arguments passed
 * 			char* argv[] - Argument vectors
 *
 * @return  void
 *
 */
static void handle_author(int argc, char* argv[])
{
	/* Author does not accept arguments */
	if(argc > 1)
	{
		printf("Invalid command sequence for : %s\n\r", argv[0]);
		return;
	}

	printf("Dhruv Mehta\n\r");
}


/**
 * @name    handle_help.
 *
 * @brief   Handles the command - help
 * 			Prints the help string for all commands
 *
 * @param	int argc - Number of arguments passed
 * 			char* argv[] - Argument vectors
 *
 * @return  void
 *
 */
static void handle_help(int argc, char* argv[])
{
	/* Help does not accept arguments */
	if(argc > 1)
	{
		printf("Invalid command sequence for : %s\n\r", argv[0]);
		return;
	}

	printf("\n\rBreakfast Serial Command Processor\n\r");

	/* Print out all help strings */
	for(int i = 0; i < num_commands; i++)
	{
		printf("%s\n\r", commands[i].help_string);
	}
}

static void handle_test(int argc, char* argv[])
{
	/* Color blue, scroll to fill screen, resize window
	 * move cursor to 1,1 */
	printf("\e[44m\n\r\e[2J\n\r\e[8;24;80t\n\r\e[1;1H");

	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("                         __   __   ___  _                                       \n\r");
	printf("               |\\/|  /\\   /  |_     |  |_ |)  |\\/| | |\\|  /\\  |            \n\r");
	printf("               |  | /  \\ /_  |__    |  |_ |\\  |  | | | | /  \\ |__            \n\r");
	printf("                                                                                \n\r");
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	printf("_______________________________________                                         \n\r");
	printf("                                       |                                        \n\r");
	printf("                  _       |       _____|                                        \n\r");
	printf("  |____________    |      |      |     |                                        \n\r");
	printf("    |              |______|  __________|                                        \n\r");
	printf("____|  |  ______   |         |         |                                        \n\r");
	printf("       |_|  |   |_________________|____|                                        \n\r");
	printf("    |       |               |                                                   \n\r");
	printf("    |_______|        ___    |          |                                        \n\r");
	printf("            |_____  |                  |                                        \n\r");
	printf("  |_________|   _   |   |____________  |                                        \n\r");
	printf("               |    |                | |                                        \n\r");
	printf("_______________|____|________________|_|                                        \n\r\e[8;1H");
}

static void handle_up(int argc, char* argv[])
{
	if(cursor_pos[ROW] != 8)
	{
		printf("\b \b\e[A*");
		cursor_pos[ROW] -= 1;
	}

	//printf("\e[7A");
	//printf("\e[6C");
	//printf("*");
}


static void handle_down(int argc, char* argv[])
{
	if(cursor_pos[ROW] != 23)
	{
		printf("\b \b\eD*");
		cursor_pos[ROW] += 1;
	}

}


static void handle_left(int argc, char* argv[])
{
	if(cursor_pos[COL] != 0)
	{
		printf("\b \b\e6*");
		cursor_pos[COL] -= 1;
	}

}


static void handle_right(int argc, char* argv[])
{
	if(cursor_pos[COL] != 78)
	{
		printf("\b \b\e9*");
		cursor_pos[COL] += 1;
	}
}

