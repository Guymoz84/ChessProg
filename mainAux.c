#include "mainAux.h"
void newgame(){
	Game* game;
	bool loaded=false;
	Settings settings=createDefaultSettings();//initialize settings
	Command command;
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	command=gameSetting(&settings);
	while(command.cmd!=START){
		if(command.cmd==LOAD){
			Game* check;
			check=load(command.fin);
			if(check!=NULL){
				game=check;
				loaded=true;
				settings=game->settings;
			}
			free(command.fin);
		}
		if(command.cmd==QUIT){
			if(loaded==true){
				gameDestroy(game);
			}
			exit(0);
			break;
		}
		command=gameSetting(&settings);
	}//Settings are valid - Good luck!
	if(loaded==false){
		game=gameCreate(HISTORY_SIZE);
		gameInitalize(game,settings);
	}
	startGame(game,loaded);
}
Game* firstMoveComputer(Game* game){
	game->currentPlayer=PLAYER_2_SYMBOL;
	Move bestmove;
	setMove(&bestmove,0,0,1,1);//Initial
	bestmove=chooseBestMove(game);
	gameSetMoveAux(game,&bestmove);
	switch(game->gameBoard[bestmove.j][bestmove.i]->type){
		case('r'): printf("Computer: move rook at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('b'): printf("Computer: move bishop at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('n'):printf("Computer: move knight at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('q'): printf("Computer: move queen at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('k'):printf("Computer: move king at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('m'):printf("Computer: move pawn at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		default:break;
	}
	changeTurn(game);
	return game;
}
void startGame(Game* game,bool loaded){
	int gameover=GAMECONTINUE;
	if(game->settings.game_mode==ONE_PLAYER_MODE){
		if(game->settings.user_color==BLACK){
			if(loaded==false){
				game->currentPlayer=PLAYER_2_SYMBOL;
			}
			if(gameGetCurrentPlayer(game)->color==WHITE){
				game=firstMoveComputer(game);
				gameover=gameOver(game);
			}
		}
		else{
			if(loaded==true){
				if(gameGetCurrentPlayer(game)->color==BLACK){
					game=firstMoveComputer(game);
					gameover=gameOver(game);
				}
			}
		}
		while(gameover==GAMECONTINUE){
			userTurn(game);
			gameover=gameOver(game);
			if(gameover==GAME_OVER){
				break;
			}
			computerTurn(game);
			gameover=gameOver(game);
		}
	}
	else{// if(game->settings.game_mode==2){
		while(gameover==GAMECONTINUE){
			userTurn(game);
			gameover=gameOver(game);
		}
	}
}
int gameOver(Game* game){
	GAME_MESSAGE message=checkWinnerOrTie(game);
	switch(message){
		case(GAME_CHECKMATE):printf("Checkmate! %s player wins the game\n",gameGetCurrentPlayer(game)->color? "black":"white"); gameDestroy(game);break;
		case(GAME_TIE):
			if(game->settings.game_mode==1&&game->currentPlayer==PLAYER_1_SYMBOL){
				printf("The game ends in a tie\n"); gameDestroy(game);break;
			}
			else{
				printf("The game is tied\n"); gameDestroy(game);break;
			}
		case(GAME_CHECK):
			if(game->settings.game_mode==1&&game->currentPlayer==PLAYER_1_SYMBOL){
				printf("Check!\n");break;
			}
			else{
				printf("Check: %s King is threatened!\n",gameGetCurrentPlayer(game)->color? "white":"black");break;
			}
		default:break;
	}
	if(message==GAME_CHECKMATE||message==GAME_TIE){
		return GAME_OVER;
	}
	return GAMECONTINUE;
}
int computerTurn(Game* game){
	Move bestmove;
	setMove(&bestmove,0,0,1,1);//Initial
	bestmove=chooseBestMove(game);
	move(bestmove,game);
	switch(game->gameBoard[bestmove.j][bestmove.i]->type){
		case('r'): printf("Computer: move rook at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('b'): printf("Computer: move bishop at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('n'):printf("Computer: move knight at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('q'): printf("Computer: move queen at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('k'):printf("Computer: move king at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		case('m'):printf("Computer: move pawn at <%d,%c> to <%d,%c>\n",GAME_N_ROWS-bestmove.y,(char)(bestmove.x+CHANGE_FOR_PRINT),GAME_N_ROWS-bestmove.j,(char)(bestmove.i+CHANGE_FOR_PRINT));break;
		default:break;
	}
	return 1;
}
void quit(Game* game){
	gameDestroy(game);
	printf("Exiting...\n");
	exit(0);
}
void restart(Game* game){
	gameDestroy(game);
	printf("Restarting...\n");
	newgame();
}
void invalid(){
	Error_Handling(E_COMMAND_LINE,"A function",0);
}
int move(Move move,Game* game){
	gameSetMove(game,move);
	changeTurn(game);
	return 1;
}
int save(Game* game,char* fileName){
	if(fileName==NULL||game==NULL){
		Error_Handling(E_FILE_SAVE,fileName,0);
		return 0;
	}
	int i,j;
	char line[MAX_LINE_LENGTH];
	FILE* file=NULL;
	file=fopen(fileName,"w");
	if(file==NULL){
		Error_Handling(E_FILE_SAVE,fileName,0);
		return 0;
	}
	fprintf(file,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n");
	fprintf(file,"\t<current_turn>%d</current_turn>\n",gameGetCurrentPlayer(game)->color);
	fprintf(file,"\t<game_mode>%d</game_mode>\n",game->settings.game_mode);
	if(game->settings.game_mode==1){
		fprintf(file,"\t<difficulty>%d</difficulty>\n",game->settings.game_level);
		fprintf(file,"\t<user_color>%d</user_color>\n",game->settings.user_color);
	}
	fprintf(file,"\t<board>\n");
	for(j=0;j<GAME_N_ROWS;j++){
		for(i=0;i<GAME_N_COLUMNS;i++){
			line[i]=getPiecePrint(game->gameBoard[j][i]);
		}
		line[GAME_N_COLUMNS]='\0';
		fprintf(file,"\t\t<row_%d>%s</row_%d>\n",GAME_N_ROWS-j,line,GAME_N_ROWS-j);
	}
	fprintf(file,"\t</board>\n</game>\n");
	fclose(file);
	return 1;
}
Game* load(char* fileName){
	int i,j;
	char line[MAX_LINE_LENGTH];
	Settings settings;
	FILE *file=NULL;
	if((file=fopen(fileName,"r"))==NULL){
		Error_Handling(E_FILE_LOAD,fileName,0);
		return NULL;
	}
	int mode,difficulty,color,currentturn;
	fscanf(file,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n");
	fscanf(file,"\t<current_turn>%d</current_turn>\n",&currentturn);
	fscanf(file,"\t<game_mode>%d</game_mode>\n",&mode);
	if(mode==1){
		fscanf(file,"\t<difficulty>%d</difficulty>\n",&difficulty);
		fscanf(file,"\t<user_color>%d</user_color>\n",&color);
	}
	else{
		difficulty=2;
		color=1;
	}
	fscanf(file,"\t<board>\n");
	settings.game_level=difficulty;
	settings.game_mode=mode;
	settings.user_color=color;
	Game* game=gameCreate(HISTORY_SIZE);
	game->settings=settings;
	checkPlayerForLoad(game,currentturn);
	game->user1=userCreate(PLAYER_1_SYMBOL,game->settings.user_color);
	game->user2=userCreate(PLAYER_2_SYMBOL,1-game->settings.user_color);
	int trash1,trash2;
	for(j=0;j<GAME_N_ROWS;j++){
		fscanf(file,"\t\t<row_%d>%s</row_%d>\n",&trash1,line,&trash2);
		for(i=0;i<GAME_N_COLUMNS;i++){
			char ch=line[i];
			if(ch==EMPTY_PIECE){
				continue;
			}
			else if(ch==toupper(ch)){//BLACK PLAYER
				loadForBlackPlayer(game,ch,j,i);
			}
			else{// WHITE PLAYER
				loadForWhitePlayer(game,ch,j,i);
			}
		}
	}
	fillEmptyPieces(game);
	fscanf(file,"\t</board>\n</game>\n");
	fclose(file);
	return game;
}
void loadForBlackPlayer(Game* game,char ch,int j,int i){
	if(game->user1->color==0){
		game->gameBoard[j][i]=addUserPiece(game->user1,i,j,tolower(ch));
		if(game->gameBoard[j][i]->type=='k'){
			game->user1->king=game->gameBoard[j][i];
		}
	}
	else{
		game->gameBoard[j][i]=addUserPiece(game->user2,i,j,tolower(ch));
		if(game->gameBoard[j][i]->type=='k'){
			game->user2->king=game->gameBoard[j][i];
		}
	}
}
void loadForWhitePlayer(Game* game,char ch,int j,int i){
	if(game->user1->color==0){
		game->gameBoard[j][i]=addUserPiece(game->user2,i,j,ch);
		if(game->gameBoard[j][i]->type=='k'){
			game->user2->king=game->gameBoard[j][i];
		}
	}
	else{
		game->gameBoard[j][i]=addUserPiece(game->user1,i,j,ch);
		if(game->gameBoard[j][i]->type=='k'){
			game->user1->king=game->gameBoard[j][i];
		}
	}
}
void fillEmptyPieces(Game* game){
	int i;
	for(i=game->user1->numpieces;i<NUM_PIECES;i++){
		game->user1->pieces[i]=NULL;
	}
	for(i=game->user2->numpieces;i<NUM_PIECES;i++){
		game->user2->pieces[i]=NULL;
	}
}
void checkPlayerForLoad(Game* game,int currentturn){
	if(currentturn==1){
		if(game->settings.user_color==WHITE){
			game->currentPlayer=PLAYER_1_SYMBOL;
		}
		else{
			game->currentPlayer=PLAYER_2_SYMBOL;
		}
	}
	else{
		if(game->settings.user_color==WHITE){
			game->currentPlayer=PLAYER_2_SYMBOL;
		}
		else{
			game->currentPlayer=PLAYER_1_SYMBOL;
		}
	}
}
int undo(Game* game){
	changeTurn(game);
	gameUndoPrevMove(game);//undoes the computer move
	changeTurn(game);
	GAME_MESSAGE message=gameUndoPrevMove(game);//undoes the user move
	if(message==GAME_SUCCESS){
		return 1;
	}
	else if(message==GAME_NO_HISTORY){
		Error_Handling(E_UNDO_MOVE,"No history",0);
		return 0;
	}
	else{
		Error_Handling(E_NULL_POINTER,"NULL",0);
		return -1;
	}
}
Command gameSetting(Settings* settings){
	char str[MAX_LINE_LENGTH];
    fgets(str,MAX_LINE_LENGTH,stdin);
    Command command=parserPraseLine(str);
    switch(command.cmd){
    case(GAME_MODE):
    	if(checkGameMode(settings,command)==false){
    		command.cmd=INVALID_LINE;
    	}
    	break;
    case(DIFFICULTY):
    	if(checkDifficulty(settings,command.arg)==false){
    		command.cmd=INVALID_LINE;
    	}
    	break;
    case(USER_COLOR):
    	if(settings->game_mode!=1){
    		command.cmd=INVALID_LINE;
    	}
    	else{
    		if(command.arg==-1){
    			command.arg=settings->user_color;
    		}
    		else if(command.arg!=1&&command.arg!=0){
    			command.cmd=INVALID_LINE;
    		}
    		else{
    			settings->user_color=command.arg;
    		}
    	}
		break;
    case(LOAD):
    	if(command.validArg==false){
    		command.cmd=LOAD;
    	}
    	break;
    case(DEFAULT):
    	setSettings(settings,DIFFICULTY_DEFAULT,ONE_PLAYER_MODE,USER_COLOR_DEFAULT);
    	break;
    case(PRINT_SETTINGS):
    	if(settings->game_mode==2){
    		printf("SETTINGS:\nGAME_MODE: 2\n");
    	}
    	else{
    		printf("SETTINGS:\nGAME_MODE: 1\nDIFFICULTY_LVL: %d\nUSER_CLR: %s\n",settings->game_level,settings->user_color? "WHITE":"BLACK");
    	}
		break;
    case(QUIT):
    	printf("Exiting...\n");
    	exit(0);
    	break;
    case(START):
    	command.cmd=START;
    	break;
    default:command.cmd=INVALID_LINE;printf("ERROR: invalid command!\n");break;
    }
	return command;
}
bool checkGameMode(Settings* settings,Command command){
	if(command.arg!=1&&command.arg!=2){
		printf("Wrong game mode\n");
		return false;
	}
	printf("Game mode is set to %d player",command.arg);
	if(command.arg==2){
		printf("s");
		settings->user_color=1;
		settings->game_mode=2;
		settings->game_level=2;
	}
	printf("\n");
	settings->game_mode=command.arg;
	return true;
}
bool checkDifficulty(Settings* settings,int arg){
	if(settings->game_mode!=1){
		return false;
	}
	if(arg>5||arg<1){
		printf("Wrong difficulty level. The value should be between 1 to 5\n");
		return false;
	}
	settings->game_level=arg;
	return true;
}
int userTurn(Game* game){
	Command command;
	printGameBoard(game);
	command.validArg=0;
	command.cmd=INVALID_LINE;
    while (command.cmd==INVALID_LINE||command.cmd==SAVE||command.cmd==GET_MOVES){
    	printf("%s player - enter your move:\n",gameGetCurrentPlayer(game)->color? "white":"black");
        command=userInput(game);
	}
    return 1;//sent to the relevant function
}
Command userInput(Game* game){//sends to the function that the user asked to perform
	char str[MAX_LINE_LENGTH];
	fgets(str,MAX_LINE_LENGTH,stdin);
	Command command;
	command=parserPraseLine(str);
	switch(command.cmd)
	{
		case(MOVE):{
			if(indexIsValid(command.move.x,command.move.y)==false||indexIsValid(command.move.i,command.move.j)==false){
				printf("Invalid position on the board\n");
				command.cmd=INVALID_LINE;
			}
			else if(game->gameBoard[command.move.y][command.move.x]==NULL||game->gameBoard[command.move.y][command.move.x]->player!=game->currentPlayer){
				printf("The specified position does not contain your piece\n");
				command.cmd=INVALID_LINE;
			}
			else if(gameIsValidMove(game,&command.move)==false){
				printf("Illegal move\n");
				command.cmd=INVALID_LINE;
			}
			else{//move is fine
				move(command.move,game);
			}
			break;
		}
		case(GET_MOVES):{
			checkGetMoves(game,command.move.x,command.move.y);
			command.cmd=GET_MOVES;
			break;
		}
		case(SAVE):{
			save(game,command.fin);
			command.cmd=SAVE;
			free(command.fin);
			break;
		}
		case(UNDO_MOVE):{
			if(game->settings.game_mode==2){
				printf("Undo command not available in 2 players mode\n");
				command.cmd=INVALID_LINE;
				break;
			}
			int message=undo(game);
			if(message==1){
				printGameBoard(game);
			}
			command.cmd=INVALID_LINE;
			break;
		}
		case(RESTART):restart(game);break;
		case(QUIT): quit(game);break;
		default:command.cmd=INVALID_LINE;invalid();break;
	}
	return command;
}
void checkGetMoves(Game* game,int x, int y){
	ArrayList* moves;
	if(game->settings.game_mode==TWO_PLAYERS_MODE||game->settings.game_level>2){
		printf("get moves command is available only in one player mode at level 1 or 2\n");
	}
	else if(indexIsValid(x,y)==false){
		printf("Invalid position on the board\n");
	}
	else if(game->gameBoard[y][x]==NULL||game->gameBoard[y][x]->player!=game->currentPlayer){
		printf("The specified position does not contain you piece\n");
	}
	else{//<x,y> if fine and difficulty is 1 or 2
		moves=getMoves(game,game->gameBoard[y][x]);
		printArrayListForGetMoves(moves,game,x,y);
	}
}
