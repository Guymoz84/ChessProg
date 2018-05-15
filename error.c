#include "error.h"
void Error_Handling(ERROR str,char* func, int num){//prints each type of error
	switch(str){
		case(E_MEMORY): printf("ERROR: %s has failed\n",func);break;
		case(E_FILE_SAVE): printf("File cannot be created or modified\n");break;
		case(E_FILE_LOAD): printf("Error: File doesn't exist or cannot be opened\n");break;
		case(E_COMMAND_LINE): printf("ERROR: invalid command\n");break;
		case(E_UNDO_MOVE): printf("Empty history, move cannot be undone\n");break;
		case(E_NULL_POINTER):printf("ERROR: there was a case of null pointer exception\n");break;
		default:;
	}
}
