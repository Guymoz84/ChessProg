#include "parser.h"
bool parserIsInt(const char* str){
	if(!str ||!*str){
		return false;
	}
    while(((*str)!='\0')&&((*str)!='\n')){
        if (!isdigit(*str)){
            return false;
        }
        else{
            ++str;
        }
    }
	return true;
}
Command parserPraseLine(const char* str){
	Command command;
	char str2[MAX_LINE_LENGTH]={0};
	strcpy(str2,str);
	char* ch=strtok(str2," \t\r\n");
	setMove(&(command.move),0,0,0,1);
	command.move.eatmove=false;
	command.validArg=false;
	command.cmd=INVALID_LINE;
	command.arg=-1;
	if(!ch){
		command.cmd=INVALID_LINE;
		return command;
	}
	else if(strcmp(ch,"move")==0){
		command=moveParser(command);
	}
	else if(strcmp(ch,"get_moves")==0){
		command=getmovesParser(command);
	}
	else if(strcmp(ch,"game_mode")==0){//game mode
		command=gamemodeParser(command);
	}
	else if(strcmp(ch,"difficulty")==0){//difficulty
		command=difficultyParser(command);
	}
	else if(strcmp(ch,"user_color")==0){
		command=usercolorParser(command);
	}
	else if(strcmp(ch,"load")==0){
		command=loadParser(command);
	}
	else if(strcmp(ch,"save")==0){
		command=saveParser(command);
	}
	else if(strtok(NULL," \t\r\n")==NULL){//no second parameter
		command=oneParamParser(command,ch);
	}
	return command;
}
Command getmovesParser(Command command){
	char* string_arg=NULL;
	char x;
	int y;
	string_arg=strtok(NULL," \t\r\n");
	if(string_arg==NULL){
		command.cmd=INVALID_LINE;
		return command;
	}
	if(sscanf(string_arg,"<%d,%c>",&y,&x)!=2){
		command.cmd=INVALID_LINE;
		return command;
	}
	command.move.y=GAME_N_ROWS-y;
	command.move.x=(int)(x-'A');
	command.cmd=GET_MOVES;
	return command;
}
Command difficultyParser(Command command){
	char* string_arg;
	int num=0;
	string_arg=strtok(NULL," \t\r\n");
	command.cmd=DIFFICULTY;
	if(parserIsInt(string_arg)){
		num=atoi(string_arg);
		command.arg=num;
	}
	command.validArg=true;
	return command;
}
Command gamemodeParser(Command command){
	char* string_arg;
	int num=0;
	string_arg=strtok(NULL," \t\r\n");
	command.cmd=GAME_MODE;
	if(parserIsInt(string_arg)){
		num=atoi(string_arg);
		command.arg=num;
	}
	command.validArg=true;
	return command;
}
Command usercolorParser(Command command){
	char* string_arg;
	int num=0;
	string_arg=strtok(NULL," \t\r\n");
	command.cmd=USER_COLOR;
	if(parserIsInt(string_arg)){
		num=atoi(string_arg);
		command.arg=num;
	}
	command.validArg=true;
	return command;
}
Command moveParser(Command command){
	char* string_arg;
	int y,j;
	char i,x;
	string_arg=strtok(NULL," \t\r\n");
	if(string_arg==NULL){
		command.cmd=INVALID_LINE;
		return command;
	}
	else if(sscanf(string_arg,"<%d,%c>",&y,&x)!=2){
		command.cmd=INVALID_LINE;
		return command;
	}
	string_arg=strtok(NULL," \t\r\n");
	if(string_arg==NULL){
		command.cmd=INVALID_LINE;
		return command;
	}
	else if(strcmp(string_arg,"to")!=0){
		command.cmd=INVALID_LINE;
		return command;
	}
	string_arg=strtok(NULL," \t\r\n");
	if(string_arg==NULL){
		command.cmd=INVALID_LINE;
		return command;
	}
	else if(sscanf(string_arg,"<%d,%c>",&j,&i)!=2){
		command.cmd=INVALID_LINE;
		return command;
	}
	command.move.y=GAME_N_ROWS-y;
	command.move.x=(int)(x-'A');
	command.move.j=GAME_N_ROWS-j;
	command.move.i=(int)(i-'A');
	command.cmd=MOVE;
	return command;
}
Command loadParser(Command command){
	char* string_arg;
	FILE *inp;
	string_arg=strtok(NULL," \t\r\n");
	command.fin=NULL;
	command.cmd=LOAD;
	inp=fopen(string_arg,"r");
	if(inp!=NULL){
		command.fin=(char*)malloc(sizeof(char)*(strlen(string_arg)+1));
		if(!command.fin){
			free(command.fin);
			return command;
		}
		strcpy(command.fin,string_arg);
		command.validArg=true;
		fclose(inp);
	}
	return command;
}
Command saveParser(Command command){
	char* string_arg;
	FILE *op;
	string_arg=strtok(NULL," \t\r\n");
	command.fin=NULL;
	command.cmd=SAVE;
	op=fopen(string_arg,"w");
	if(op!=NULL){
		command.fin=(char*)malloc(sizeof(char)*(strlen(string_arg)+1));
		if(!command.fin){
			free(command.fin);
			return command;
		}
		strcpy(command.fin,string_arg);
		command.validArg=true;
		fclose(op);
	}
	return command;
}
Command oneParamParser(Command command,char* ch){
	if(strcmp(ch,"undo")==0){
		command.cmd=UNDO_MOVE;
		command.validArg=true;
	}
	else if(strcmp(ch,"default")==0){
		command.cmd=DEFAULT;
		command.validArg=true;
	}
	else if(strcmp(ch,"reset")==0){
		command.cmd=RESTART;
		command.validArg=true;
	}
	else if(strcmp(ch,"print_setting")==0){
		command.cmd=PRINT_SETTINGS;
		command.validArg=true;
	}
	else if(strcmp(ch,"quit")==0){
		command.cmd=QUIT;
		command.validArg=true;
	}
	else if(strcmp(ch,"start")==0){
		command.cmd=START;
		command.validArg=true;
	}
	return command;
}
