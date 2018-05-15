#ifndef PARSER_H_
#define PARSER_H_
#include "ai.h"
//specify the maximum line length
//#define MAX_LINE_LENGTH 1024
//a type used to represent a command
typedef enum command_e{
	GAME_MODE,
	DIFFICULTY,
	USER_COLOR,
	LOAD,
	DEFAULT,
	PRINT_SETTINGS,
	QUIT,
	RESTART,
	UNDO_MOVE,
	START,
	MOVE,
	SAVE,
	INVALID_LINE,
	GET_MOVES,
} COMMAND_E;

//a new type that is used to encapsulate a parsed line
typedef struct command_t{
	COMMAND_E cmd;
	bool validArg; //is set to true if the line contains a valid argument
	int arg;
	Move move;
	char* fin;
} Command;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool parserIsInt(const char* str);

/**
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */
Command parserPraseLine(const char* str);
/**
 * Takes the input from user, checks if valid and divide to the choice.
 * @param - command - after initialize.
 * @return
 * The command from the input, depends on the option that has been chosen.
 */
Command getmovesParser(Command command);
Command difficultyParser(Command command);
Command gamemodeParser(Command command);
Command usercolorParser(Command command);
Command moveParser(Command command);
Command loadParser(Command command);
Command saveParser(Command command);
Command oneParamParser(Command command,char* ch);

#endif
