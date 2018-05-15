#ifndef ERROR_H_
#define ERROR_H_
#include <stdio.h>

typedef enum {
	E_MEMORY,
	E_COMMAND_LINE,
	E_UNDO_MOVE,
	E_NULL_POINTER,
	E_FILE_SAVE,
	E_FILE_LOAD
} ERROR;

/*
 * Error handling function, gets a ERROR and acts accordingly to cases
 * @param str - the type of error, from the enum ERROR
 * @param func - the name of the function the error was accured there
 * @param num - additional number to specify in case the error has kind of a special index to notify
 */
void Error_Handling(ERROR str,char* func, int num);
#endif /* ERROR_H_ */
