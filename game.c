#include "game.h"
Game* gameCreate(int historySize){
	int i,j;
	if(historySize<=0){
		Error_Handling(E_COMMAND_LINE,"GameCreate",0);
		return NULL;
	}
	Game* game=(Game*) malloc(sizeof(Game));
	if(!game){
		Error_Handling(E_MEMORY,"GameCreate",0);
		gameDestroy(game);
		exit(0);
		return NULL;
	}
	game->currentPlayer=PLAYER_1_SYMBOL;
	game->history= arrayListCreate(historySize);
	if(!(game->history)){
		gameDestroy(game);
		Error_Handling(E_MEMORY,"GameCreate",0);
		exit(0);
		return NULL;
	}
	for(i=0;i<GAME_N_COLUMNS;i++){
		for(j=0;j<GAME_N_ROWS;j++){
			game->gameBoard[j][i]=NULL;
		}
	}
	return game;
}
Game* gameInitalize(Game* game,Settings settings){
	int i;
	if(settings.game_mode==TWO_PLAYERS_MODE){
		game->settings.user_color=WHITE;
	}
	setSettings(&(game->settings),settings.game_level,settings.game_mode,settings.user_color);
	game->user1=(User*)userCreate(PLAYER_1_SYMBOL,game->settings.user_color);
	if(!game->user1){
		Error_Handling(E_MEMORY,"User1",0);
		gameDestroy(game);
		exit(0);
		return NULL;
	}
	game->user2=(User*)userCreate(PLAYER_2_SYMBOL,1-game->settings.user_color);
	if(!game->user2){
		Error_Handling(E_MEMORY,"User2",0);
		gameDestroy(game);
		exit(0);
		return NULL;
	}
	game->user1=userInitalize(game->user1);
	game->user2=userInitalize(game->user2);
	for(i=0;i<NUM_PIECES;i++){
		game->gameBoard[game->user1->pieces[i]->y][game->user1->pieces[i]->x]=game->user1->pieces[i];
		game->gameBoard[game->user2->pieces[i]->y][game->user2->pieces[i]->x]=game->user2->pieces[i];
	}
	if(settings.user_color==BLACK){
		game->currentPlayer=PLAYER_2_SYMBOL;
	}
	else{
		game->currentPlayer=PLAYER_1_SYMBOL;
	}
	return game;
}
void gameDestroy(Game* game){
	if(!game){
		return;
	}
	userDestroy(game->user1);
	userDestroy(game->user2);
	arrayListDestroy(game->history);
	free(game);
	return;
}
GAME_MESSAGE gameSetMove(Game* game, Move move){
	if (arrayListIsFull(game->history)){
		if(arrayListRemoveFirst(game->history)==ARRAY_LIST_INVALID_ARGUMENT){
			return GAME_INVALID_MOVE;
		}
	}
	gameSetMoveAux(game,&move);
	arrayListAddLast(game->history,move);
	return GAME_SUCCESS;
}
int gameSetMoveAux(Game* game,Move* move){
	move->eatmove=false;
	move->eaten=NULL;
	if(game->gameBoard[move->j][move->i]!=NULL){//an eat move
		move->eaten=game->gameBoard[move->j][move->i];
		move->eatmove=true;
		move->eaten->alive=false;
	}
	game->gameBoard[move->j][move->i]=game->gameBoard[move->y][move->x];
	game->gameBoard[move->y][move->x]=NULL;
	movePiece(game->gameBoard[move->j][move->i], move->i,move->j);
	return 1;
}
bool gameIsValidMove(Game* game,Move* move){
	if(!indexIsValid(move->i,move->j)||!indexIsValid(move->y,move->x)){
		return false;
	}
	if(game->gameBoard[move->y][move->x]==NULL){
		return false;
	}
	if(game->gameBoard[move->y][move->x]->color!=gameGetCurrentPlayer(game)->color){
		return false;
	}
	if(game->gameBoard[move->j][move->i]!=NULL){
		if(game->gameBoard[move->y][move->x]->color==game->gameBoard[move->j][move->i]->color){
			return false;
		}
	}
	bool pseudolegal=false;
	bool legal=false;
	pseudolegal=gamePseudoLegalMove(game, *move);
	if(pseudolegal==true){//pseudo legal move now needs to check if the player's king isn't threatened
		gameSetMoveAux(game,move);//moves the piece
		if(gameCheck(game)==false){//legal move(the king is not threatened)
			legal=true;
		}
		undoLastMoveAux(game,move);
	}
	return legal;
}
bool gamePseudoLegalMove(Game* game, Move move){
	bool pseudolegal=false;
	switch(game->gameBoard[move.y][move.x]->type){
		case('r'): pseudolegal=validMoveRook(game,move);break;
		case('b'): pseudolegal=validMoveBishop(game,move);break;
		case('n'): pseudolegal=validMoveKnight(game,move);break;
		case('q'): pseudolegal=validMoveQueen(game,move);break;
		case('k'): pseudolegal=validMoveKing(game,move);break;
		case('m'): pseudolegal=validMovePawn(game,move);break;
		default: pseudolegal=false; break;
	}
	return pseudolegal;
}
GAME_MESSAGE gameUndoPrevMove(Game* game){
	if(!game){
		Error_Handling(E_NULL_POINTER,NULL,0);
		return GAME_INVALID_ARGUMENT;
	}
	if(arrayListIsEmpty(game->history)){
		return GAME_NO_HISTORY;
	}
	undoLastMove(game);
	return GAME_SUCCESS;
}
void undoLastMove(Game* game){
	Move move = arrayListGetLast(game->history);
	ARRAY_LIST_MESSAGE message=arrayListRemoveLast(game->history);
	if(message==ARRAY_LIST_SUCCESS){
		undoLastMoveAux(game,&move);
		printf("Undo move for player %s : <%d,%c> -> <%d,%c>\n",game->gameBoard[move.y][move.x]->color?"white":"black",GAME_N_ROWS-move.j,move.i+CHANGE_FOR_PRINT,GAME_N_ROWS-move.y,move.x+CHANGE_FOR_PRINT);
	}
	else if(message==ARRAY_LIST_EMPTY){
		printf("Empty history, move cannot be undone\n");
	}
}
void undoLastMoveAux(Game* game,Move* move){
	game->gameBoard[move->y][move->x]=game->gameBoard[move->j][move->i];
	movePiece(game->gameBoard[move->y][move->x], move->x,move->y);
	game->gameBoard[move->j][move->i]=NULL;
	if(move->eatmove==true){
		move->eaten->alive=true;
		game->gameBoard[move->j][move->i]=move->eaten;
	}
}
void changeTurn(Game* game){
	if(game->currentPlayer==PLAYER_1_SYMBOL){
		game->currentPlayer=PLAYER_2_SYMBOL;
	}
	else{
		game->currentPlayer=PLAYER_1_SYMBOL;
	}
}
GAME_MESSAGE printGameBoard(Game* game){
	int i,j;
	if(!game){
		return GAME_INVALID_ARGUMENT;
	}
	for(j=0;j<GAME_N_ROWS;j++){
		printf("%d|",GAME_N_ROWS-j);
		for(i=0;i<GAME_N_COLUMNS;i++){
			printf(" %c",getPiecePrint(game->gameBoard[j][i]));
		}
		printf(" |\n");
	}
	printf("  ");
	for(i=1;i<=(2*GAME_N_COLUMNS+1);i++){
		printf("-");
	}
	printf("\n   ");
	for(i=1;i<GAME_N_COLUMNS;i++){
		printf("%c ",i+64);
	}
	printf("%c",GAME_N_COLUMNS+64);
	printf("\n");
	return GAME_SUCCESS;
}
User* gameGetCurrentPlayer(Game* game){
	if(!game){
		Error_Handling(E_NULL_POINTER,NULL,0);
		return NULL;
	}
	else if(game->currentPlayer==PLAYER_1_SYMBOL){
		return game->user1;
	}
	return game->user2;
}
GAME_MESSAGE checkWinnerOrTie(Game* game){
	if(gameCheck(game)){
		if(gameCheckmate(game)){
			return GAME_CHECKMATE;
		}
		return GAME_CHECK;
	}
	if(gameTie(game)){
		return GAME_TIE;
	}
	return GAME_CONTINUE;//the game continues
}
bool validMoveRook(Game* game,Move move){
	int n;
	if(move.y!=move.j && move.x!=move.i){
		return false;
	}
	if(move.y==move.j && move.x!=move.i){//left/right
		if(move.x>move.i){//moves left
			for(n=(move.x-1);n>=move.i+1;n--){
				if(game->gameBoard[move.y][n]!=NULL){
					return false;
				}
			}
		}
		else{//moves right
				for(n=move.x+1;n<=move.i-1;n++){
					if(game->gameBoard[move.y][n]!=NULL){
						return false;
					}
				}
			}
	}
	else if(move.y!=move.j && move.x==move.i){//up/down
		if(move.y>move.j){//moves up
			for(n=(move.y-1);n>=move.j+1;n--){
				if(game->gameBoard[n][move.x]!=NULL){
					return false;
				}
			}
		}
		else{//moves down
				for(n=move.y+1; n<=move.j-1; n++){
					if(game->gameBoard[n][move.x]!=NULL){
						return false;
					}
				}
			}
	}
	return true;
}
bool validMoveBishop(Game* game,Move move){
	int n;
	if(move.y>move.j&&move.x<move.i){//up right
		n=1;
		while(indexIsValid(move.x+n,move.y-n)){
			if(move.y-n==move.j&&move.x+n==move.i){
				return true;
			}
			else if(game->gameBoard[move.y-n][move.x+n]!=NULL){
				return false;
			}
			n++;
		}
		return false;
	}
	else if(move.y>move.j&&move.x>move.i){//up left
		n=1;
		while(indexIsValid(move.x-n,move.y-n)){
			if(move.y-n==move.j&&move.x-n==move.i){
				return true;
			}
			else if(game->gameBoard[move.y-n][move.x-n]!=NULL){
				return false;
			}
			n++;
		}
		return false;
	}
	else if(move.y<move.j&&move.x<move.i){//down right
		n=1;
		while(indexIsValid(move.x+n,move.y+n)){
			if(move.y+n==move.j&&move.x+n==move.i){
				return true;
			}
			else if(game->gameBoard[move.y+n][move.x+n]!=NULL){
				return false;
			}
			n++;
		}
		return false;
	}
	else if(move.y<move.j&&move.x>move.i){//down left
		n=1;
		while(indexIsValid(move.x-n,move.y+n)){
			if(move.y+n==move.j&&move.x-n==move.i){
				return true;
			}
			else if(game->gameBoard[move.y+n][move.x-n]!=NULL){
				return false;
			}
			n++;
		}
		return false;
	}
	return false;
}
bool validMoveKnight(Game* game,Move move){
	if((move.y+1==move.j)&&((move.x+2==move.i)||(move.x-2==move.i))){
		return true;
	}
	else if((move.y+2==move.j)&&((move.x+1==move.i)||(move.x-1==move.i))){
		return true;
	}
	else if((move.y-1==move.j)&&((move.x+2==move.i)||(move.x-2==move.i))){
		return true;
	}
	else if((move.y-2==move.j)&&((move.x+1==move.i)||(move.x-1==move.i))){
		return true;
	}
	return false;
}
bool validMoveQueen(Game* game,Move move){
	if(validMoveBishop(game,move)||validMoveRook(game,move)){
		return true;
	}
	return false;
}
bool validMoveKing(Game* game,Move move){
	if((move.y+1==move.j)&&((move.x+1==move.i)||(move.x-1==move.i)||(move.x==move.i))){//3 options
		return true;
	}
	else if((move.y==move.j)&&((move.x+1==move.i)||(move.x-1==move.i))){//2 options
		return true;
	}
	else if((move.y-1==move.j)&&((move.x+1==move.i)||(move.x-1==move.i)||(move.x==move.i))){//3 options
		return true;
	}
	return false;
}
bool validMovePawn(Game* game,Move move){
	User* current =gameGetCurrentPlayer(game);
	if(current->color==WHITE){
		return validMovePawnWhite(game,move);
	}
	else if(current->color==BLACK){
		return validMovePawnBlack(game,move);
	}
	return false;
}
bool validMovePawnBlack(Game* game,Move move){
	char player=game->gameBoard[move.y][move.x]->player;
	if(move.j==move.y+1&&move.x==move.i){//doesn't eat
		if(game->gameBoard[move.j][move.i]==NULL){
			return true;
		}
		else{
			return false;
		}
	}
	else if(move.y==1&&move.x==move.i){//pawn can move 2 steps at its first position
		if(move.j==move.y+2){
			if(game->gameBoard[move.j][move.i]==NULL&&game->gameBoard[move.y+1][move.x]==NULL){
				return true;
			}
			else{
				return false;
			}
		}
	}
	else{//eats
		if((move.j==move.y+1)&&((move.x+1==move.i)||(move.x-1==move.i))){
			if(game->gameBoard[move.j][move.i]!=NULL){
				if(game->gameBoard[move.j][move.i]->player!=player){
					return true;
				}
				else{
					return false;
				}
			}
		}
	}
	return false;
}
bool validMovePawnWhite(Game* game,Move move){
	char player=game->gameBoard[move.y][move.x]->player;
	if(move.j==move.y-1&&move.x==move.i){//doesn't eat
		if(game->gameBoard[move.j][move.i]==NULL){
			return true;
		}
		else{
			return false;
		}
	}
	else if(move.y==GAME_N_ROWS-2&&move.x==move.i){//pawn can move 2 steps at its first position
		if(move.j==move.y-2){
			if(game->gameBoard[move.j][move.i]==NULL&&game->gameBoard[move.y-1][move.x]==NULL){
				return true;
			}
			else{
				return false;
			}
		}
	}
	else{//eats
		if((move.j==move.y-1)&&((move.x+1==move.i)||(move.x-1==move.i))){
			if(game->gameBoard[move.j][move.i]!=NULL){
				if(game->gameBoard[move.j][move.i]->player!=player){
					return true;
				}
				else{
					return false;
				}
			}
		}
	}
	return false;
}
ArrayList* getMoves(Game* game,Piece* piece){
	switch(piece->type){
		case('r'): return getMovesRook(game,piece);break;
		case('b'): return getMovesBishop(game,piece);break;
		case('n'): return getMovesKnight(game,piece);break;
		case('q'): return getMovesQueen(game,piece);break;
		case('k'): return getMovesKing(game,piece);break;
		case('m'): return getMovesPawn(game,piece);break;
		default: return NULL; break;
	}
}
ArrayList* getMovesRook(Game* game,Piece* piece){
	int i;
	ArrayList* moves=arrayListCreate(14);
	for(i=0;i<GAME_N_ROWS;i++){
		if(piece->x!=i){
			setAndInsert(game,moves,piece,piece->x,piece->y,i,piece->y);
		}
		if(piece->y!=i){
			setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,i);
		}
	}
	return moves;
}
ArrayList* getMovesBishop(Game* game,Piece* piece){
	int i;
	ArrayList* moves=arrayListCreate(13);
	for(i=0;i<GAME_N_ROWS;i++){
		setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+i,piece->y+i);
		setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-i,piece->y-i);
		setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+i,piece->y-i);
		setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-i,piece->y+i);
	}
	return moves;
}
ArrayList* getMovesKnight(Game* game,Piece* piece){
	ArrayList* moves=arrayListCreate(8);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+2,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+2,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-2,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-2,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y+2);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y-2);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y+2);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y-2);
	return moves;
}
ArrayList* getMovesQueen(Game* game,Piece* piece){
	int i;
	ArrayList* moves=arrayListCreate(27);
	ArrayList* movesAsRook=getMovesRook(game,piece);
	ArrayList* movesAsBishop=getMovesBishop(game,piece);
	if(movesAsRook!=NULL){
		for(i=0;i<arrayListSize(movesAsRook);i++){
			arrayListAddLast(moves,arrayListGetAt(movesAsRook,i));
		}
	}
	arrayListDestroy(movesAsRook);
	if(movesAsBishop!=NULL){
		for(i=0;i<arrayListSize(movesAsBishop);i++){
			arrayListAddLast(moves,arrayListGetAt(movesAsBishop,i));
		}
	}
	arrayListDestroy(movesAsBishop);
	return moves;
}
ArrayList* getMovesKing(Game* game,Piece* piece){
	ArrayList* moves=arrayListCreate(8);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y+1);
	return moves;
}
ArrayList* getMovesPawn(Game* game,Piece* piece){
	ArrayList* moves=arrayListCreate(4);
	User* user =gameGetCurrentPlayer(game);
	if(user->color==WHITE){
		getMovesPawnWhite(game,moves,piece);
	}
	else if(user->color==BLACK){
		getMovesPawnBlack(game,moves,piece);
	}
	return moves;
}
ArrayList* getMovesPawnWhite(Game* game,ArrayList* moves,Piece* piece){
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y-1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y-2);
	return moves;
}
ArrayList* getMovesPawnBlack(Game* game,ArrayList* moves,Piece* piece){
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x+1,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x-1,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y+1);
	setAndInsert(game,moves,piece,piece->x,piece->y,piece->x,piece->y+2);
	return moves;
}
void setAndInsert(Game* game,ArrayList* moves,Piece* piece,int x,int y,int i,int j){
	Move move;
	setMove(&move,x,y,i,j);
	insertMoves(game,move,moves);
}
void insertMoves(Game* game,Move move,ArrayList* moves){
	bool legal;
	legal=gameIsValidMove(game,&move);
	if(legal==true){
		arrayListAddLast(moves,move);
	}
}
bool gameCheck(Game* game){
	int i,j;
	User* user=gameGetCurrentPlayer(game);
	Piece* kingthreat;
	i=user->king->x;
	j=user->king->y;
	kingthreat=gameThreat(game,i,j);
	if(kingthreat!=NULL){
		return true;
	}
	return false;
}
Piece* gameThreat(Game* game,int i,int j){//i,j the threatened piece
	Piece* threat;
	threat=threatByBishop(game,i,j,'b');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByRook(game,i,j,'r');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByKnight(game,i,j,'n');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByPawn(game,i,j,'m');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByQueen(game,i,j,'q');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByKing(game,i,j,'k');
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatByPawn(Game* game,int i,int j,char type){
	Piece* threat;
	User* user =gameGetCurrentPlayer(game);
	if(user->color==WHITE){
		threat=threatByBlackPawn(game,i,j,'m');
		if(threat!=NULL){
			return threat;
		}
	}
	else if(user->color==BLACK){
		threat=threatByWhitePawn(game,i,j,'m');
		if(threat!=NULL){
			return threat;
		}
	}
	return NULL;
}
Piece* threatByWhitePawn(Game* game,int i,int j,char type){
	Piece* threat=findThreatBasic(game,i,j,-1,1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,1,1,type);
	return threat;
}
Piece* threatByBlackPawn(Game* game,int i,int j,char type){
	Piece* threat=findThreatBasic(game,i,j,-1,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,1,-1,type);
	return threat;
}
Piece* threatByBishop(Game* game,int i,int j,char type){
	Piece* threat;
	threat=threatDiagToLeftUpBishop(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatDiagToRightUpBishop(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatDiagToLeftDownBishop(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatDiagToRightDownBishop(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatDiagToLeftUpBishop(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,-1,-1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatDiagToRightUpBishop(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,1,-1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatDiagToLeftDownBishop(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,-1,1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatDiagToRightDownBishop(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,1,1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatByKnight(Game* game,int i,int j,char type){
	Piece* threat=findThreatBasic(game,i,j,1,-2,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,2,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,1,2,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-1,2,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,2,1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-2,1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-2,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-1,-2,type);
	return threat;
}
Piece* threatByRook(Game* game,int i,int j,char type){
	Piece* threat;
	threat=threatUpRook(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatDownRook(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatLeftRook(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	threat=threatRightRook(game,i,j,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatUpRook(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,0,-1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatDownRook(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,0,1,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatLeftRook(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,-1,0,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatRightRook(Game* game,int i,int j,char type){
	Piece* threat=findThreat(game,i,j,1,0,type);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatByQueen(Game* game,int i,int j,char type){
	Piece* threat;
	threat=threatByRook(game,i,j,'q');
	if(threat!=NULL){
		return threat;
	}
	threat=threatByBishop(game,i,j,'q');
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
Piece* threatByKing(Game* game,int i,int j,char type){
	Piece* threat=findThreatBasic(game,i,j,1,1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,1,0,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,1,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,0,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-1,-1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-1,0,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,-1,1,type);
	if(threat!=NULL){
		return threat;
	}
	threat=findThreatBasic(game,i,j,0,1,type);
	return threat;
}
Piece* threatBasic(Game* game,Move move){//x,y threat - i,j target
	if(indexIsValid(move.x,move.y)){
		if(game->gameBoard[move.y][move.x]!=NULL){//there is a piece in there
			if(game->gameBoard[move.y][move.x]->player!=game->currentPlayer){//piece of the opponent
				if(game->gameBoard[move.j][move.i]!=NULL&&game->gameBoard[move.j][move.i]->type=='k'){//direct threat on the king
					changeTurn(game);
					if(gamePseudoLegalMove(game,move)==true){
						changeTurn(game);
						return game->gameBoard[move.y][move.x];
					}
					changeTurn(game);
				}
				else{
					changeTurn(game);
					if(gameIsValidMove(game,&move)==true){//valid move
						changeTurn(game);
						return game->gameBoard[move.y][move.x];
					}
					changeTurn(game);
				}
			}
		}
	}
	return NULL;
}
Piece* findThreat(Game* game,int i,int j,int sum1,int sum2,char type){//sum1=how much to add to move.x, sum2=how much to add to move.y
	Move move;
	int k;
	Piece* threat;
	setMove(&move,i+sum1,j+sum2,i,j);
	for(k=0;k<GAME_N_ROWS;k++){
		if(indexIsValid(move.x,move.y)==true){
			threat=threatBasic(game,move);
			if(threat!=NULL){
				return threat;
			}
		}
		move.x+=sum1;
		move.y+=sum2;
	}
	return NULL;
}
Piece* findThreatBasic(Game* game,int i,int j,int sum1,int sum2,char type){//sum1=how much to add to move.x, sum2=how much to add to move.y
	Move move;
	setMove(&move,0,0,0,1);
	Piece* threat;
	setMove(&move,i+sum1,j+sum2,i,j);
	threat=threatBasic(game,move);
	if(threat!=NULL){
		return threat;
	}
	return NULL;
}
bool gameCheckmate(Game* game){
	int i;
	Move move;
	User* current=gameGetCurrentPlayer(game);
	Piece* kingthreat=gameThreat(game,current->king->x,current->king->y);
	if(kingthreat==NULL){
		return false;
	}
	bool canblock=false;
	ArrayList* kingmoves=getMovesKing(game,current->king);
	int numberofkingsmoves=kingmoves->actualSize;
	arrayListDestroy(kingmoves);
	if(numberofkingsmoves>0)//king can escape the check
		return false;
	if(game->user1==current){//there is at least one threat on the king and he can't move
		for(i=0;i<game->user2->numpieces;i++){
			if(game->user2->pieces[i]!=kingthreat){
				move.x=game->user2->pieces[i]->x;
				move.y=game->user2->pieces[i]->y;
				move.i=current->king->x;
				move.j=current->king->y;
				changeTurn(game);
				if(gameIsValidMove(game,&move)==true){
					changeTurn(game);
					current->king->alive=false;
					return true;
				}
				changeTurn(game);
			}}}
	else{//there is at least one threat on the king and he can't move
		for(i=0;i<game->user1->numpieces;i++){
			if(game->user1->pieces[i]!=kingthreat){
				move.x=game->user1->pieces[i]->x;
				move.y=game->user1->pieces[i]->y;
				move.i=current->king->x;
				move.j=current->king->y;
				changeTurn(game);
				if(gameIsValidMove(game,&move)==true){
					changeTurn(game);
					current->king->alive=false;
					return true;
				}
				changeTurn(game);
			}}}
	//only one threat
	Piece* piecethreat;
	changeTurn(game);
	piecethreat=gameThreat(game,kingthreat->x,kingthreat->y);
	if(piecethreat!=NULL){//can eat the threat
		changeTurn(game);
		return false;
	}
	changeTurn(game);
	canblock=gameBlockThreat(game,current->king,kingthreat);
	if(canblock==true){
		return false;
	}
	current->king->alive=false;
	return true;
}
bool gameBlockThreat(Game* game,Piece* king,Piece* kingthreat){
	switch(kingthreat->type){
		case('r'): return gameBlockThreatRook(game,king,kingthreat);break;
		case('b'): return gameBlockThreatBishop(game,king,kingthreat);break;
		case('q'): return gameBlockThreatQueen(game,king,kingthreat);break;
		default: return false; break;//you can't block the knight or the pawn
	}
}
bool gameBlockThreatBishop(Game* game,Piece* king,Piece* kingthreat){
	int i;
	changeTurn(game);
	bool canblock=false;
	if(king->x<kingthreat->x&&king->y>kingthreat->y){//check from up/right
		for(i=1;i<king->y-kingthreat->y;i++){
			if(gameThreat(game,king->x+i,king->y-i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x>kingthreat->x&&king->y>kingthreat->y){//check from up/left
		for(i=1;i<=king->y-kingthreat->y;i++){
			if(gameThreat(game,king->x-i,king->y-i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x<kingthreat->x&&king->y<kingthreat->y){//check from down/right
		for(i=1;i<=kingthreat->y-king->y;i++){
			if(gameThreat(game,king->x+i,king->y+i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x>kingthreat->x&&king->y<kingthreat->y){//check from down/left
		for(i=1;i<=kingthreat->y-king->y;i++){
			if(gameThreat(game,king->x-i,king->y+i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	changeTurn(game);
	return canblock;
}
bool gameBlockThreatRook(Game* game,Piece* king,Piece* kingthreat){
	int i;
	changeTurn(game);
	bool canblock=false;
	if(king->x==kingthreat->x&&king->y>kingthreat->y){//check from up
		for(i=king->y-1;i>=kingthreat->y;i--){
			if(gameThreat(game,king->x,i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x==kingthreat->x&&king->y<kingthreat->y){//check from down
		for(i=king->y+1;i<=kingthreat->y;i++){
			if(gameThreat(game,king->x,i)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x>kingthreat->x&&king->y==kingthreat->y){//check from left
		for(i=king->x-1;i>=kingthreat->x;i--){
			if(gameThreat(game,i,king->y)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	else if(king->x<kingthreat->x&&king->y==kingthreat->y){//check from right
		for(i=king->x+1;i<=kingthreat->x;i++){
			if(gameThreat(game,i,king->y)!=NULL){
				canblock=true;
				break;
			}
		}
	}
	changeTurn(game);
	return canblock;
}
bool gameBlockThreatQueen(Game* game,Piece* king,Piece* kingthreat){
	if(gameBlockThreatRook(game,king,kingthreat)||gameBlockThreatBishop(game,king,kingthreat)){
		return true;
	}
	return false;
}
bool gameTie(Game* game){
	int i;
	ArrayList* moves;
	int numberofmoves;
	User* user=gameGetCurrentPlayer(game);
	for(i=0;i<user->numpieces;i++){
		if(user->pieces[i]->alive==true&&user->pieces[i]->type!='k'){
			moves=getMoves(game,user->pieces[i]);
			numberofmoves=arrayListSize(moves);
			arrayListDestroy(moves);
			if(numberofmoves!=0){
				return false;
			}
		}
	}
	return true;
}
bool indexIsValid(int x,int y){
	if(y>=0&&y<=GAME_N_ROWS-1&&x>=0&&x<=GAME_N_COLUMNS-1){
		return true;
	}
	return false;
}
void printArrayListForGetMoves(ArrayList* arraylist,Game* game,int x,int y){
	int k,num=0;
	arraylist=sortForGetMoves(arraylist);
	for(k=0;k<arraylist->actualSize;k++){
		num=checkIfEatMove(arraylist->elements[k]);
		gameSetMoveAux(game,&arraylist->elements[k]);
		if(gameThreat(game,arraylist->elements[k].i,arraylist->elements[k].j)!=NULL){//threat by opponent
			if(num==2){//state 3 in the instructions for get moves
				printf("<%d,%c>*^\n",GAME_N_ROWS-arraylist->elements[k].j,arraylist->elements[k].i+CHANGE_FOR_PRINT);
			}
			else{//state 1
				printf("<%d,%c>*\n",GAME_N_ROWS-arraylist->elements[k].j,arraylist->elements[k].i+CHANGE_FOR_PRINT);
			}
		}
		else{
			if(num==2){//state 2
				printf("<%d,%c>^\n",GAME_N_ROWS-arraylist->elements[k].j,arraylist->elements[k].i+CHANGE_FOR_PRINT);
			}
			else{//non eat move and not threatened
				printf("<%d,%c>\n",GAME_N_ROWS-arraylist->elements[k].j,arraylist->elements[k].i+CHANGE_FOR_PRINT);
			}
		}
	undoLastMoveAux(game,&arraylist->elements[k]);
	}
}
