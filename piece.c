#include "piece.h"
int pawnTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			   {{ 0,  0,  0,  0,  0,  0,  0,  0},
				{50, 50, 50, 50, 50, 50, 50, 50},
				{10, 10, 20, 30, 30, 20, 10, 10},
				{ 5,  5, 10, 25, 25, 10,  5,  5},
				{ 0,  0,  0, 20, 20,  0,  0,  0},
				{ 5, -5,-10,  0,  0,-10, -5,  5},
				{ 5, 10, 10,-20,-20, 10, 10,  5},
				{ 0,  0,  0,  0,  0,  0,  0,  0}};
int knightTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			   {{-50,-40,-30,-30,-30,-30,-40,-50},
				{-40,-20,  0,  0,  0,  0,-20,-40},
				{-30,  0, 10, 15, 15, 10,  0,-30},
				{-30,  5, 15, 20, 20, 15,  5,-30},
				{-30,  0, 15, 20, 20, 15,  0,-30},
				{-30,  5, 10, 15, 15, 10,  5,-30},
				{-40,-20,  0,  5,  5,  0,-20,-40},
				{-50,-40,-30,-30,-30,-30,-40,-50}};
int bishopTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			   {{-20,-10,-10,-10,-10,-10,-10,-20},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-10,  0,  5, 10, 10,  5,  0,-10},
				{-10,  5,  5, 10, 10,  5,  5,-10},
				{-10,  0, 10, 10, 10, 10,  0,-10},
				{-10, 10, 10, 10, 10, 10, 10,-10},
				{-10,  5,  0,  0,  0,  0,  5,-10},
				{-20,-10,-10,-10,-10,-10,-10,-20}};
int rookTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			 {{ 0,  0,  0,  0,  0,  0,  0,  0},
			  { 5, 10, 10, 10, 10, 10, 10,  5},
			  {-5,  0,  0,  0,  0,  0,  0, -5},
			  {-5,  0,  0,  0,  0,  0,  0, -5},
			  {-5,  0,  0,  0,  0,  0,  0, -5},
			  {-5,  0,  0,  0,  0,  0,  0, -5},
			  {-5,  0,  0,  0,  0,  0,  0, -5},
			  { 0,  0,  0,  5,  5,  0,  0,  0}};
int queenTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			   {{-20,-10,-10, -5, -5,-10,-10,-20},
				{-10,  0,  0,  0,  0,  0,  0,-10},
				{-10,  0,  5,  5,  5,  5,  0,-10},
				{ -5,  0,  5,  5,  5,  5,  0, -5},
				{  0,  0,  5,  5,  5,  5,  0, -5},
				{-10,  5,  5,  5,  5,  5,  0,-10},
				{-10,  0,  5,  0,  0,  0,  0,-10},
				{-20,-10,-10, -5, -5,-10,-10,-20}};
int kingTable[GAME_N_ROWS][GAME_N_COLUMNS]=
			   {{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-30,-40,-40,-50,-50,-40,-40,-30},
				{-20,-30,-30,-40,-40,-30,-30,-20},
				{-10,-20,-20,-20,-20,-20,-20,-10},
				{ 20, 20,  0,  0,  0,  0, 20, 20},
				{ 20, 30, 10,  0,  0, 10, 30, 20}};
Piece* createPiece(int x,int y,char player,char type,int color){
	Piece* piece=(Piece*)malloc(sizeof(Piece));
	if(!piece){
		destroyPiece(piece);
		exit(0);
	}
	setPiece(piece,x,y,player,type,color,true,false);
	return piece;
}
Piece* pieceCopy(Piece* piece){
	Piece* copy=createPiece(piece->x,piece->y,piece->player,piece->type,piece->color);
	copy->alive=piece->alive;
	return copy;
}
void destroyPiece(Piece* piece){
	free(piece);
}
int setPiece(Piece* piece,int x,int y,char player,char type,int color,bool alive,bool moved){
	setPieceX(piece,x);
	setPieceY(piece,y);
	setPiecePlayer(piece,player);
	setPieceType(piece,type);
	setPieceColor(piece,color);
	setPieceAlive(piece,alive);
	return 1;
}
int setPieceX(Piece* piece,int x){
	piece->x=x;
	return 1;
}
int setPieceY(Piece* piece,int y){
	piece->y=y;
	return 1;
}
int setPiecePlayer(Piece* piece,char player){
	piece->player=player;
	return 1;
}
int setPieceType(Piece* piece,char type){
	piece->type=type;
	return 1;
}
int setPieceColor(Piece* piece,int color){
	piece->color=color;
	return 1;
}
int setPieceAlive(Piece* piece,bool alive){
	piece->alive=alive;
	return 1;
}
int getPieceScore(Piece* piece){
	if(piece->alive==true){
		switch(piece->type){
			case('k'): return 100;break;
			case('q'): return 9;break;
			case('r'): return 5;break;
			case('b'): return 3;break;
			case('n'): return 3;break;
			case('m'): return 1;break;
			default: return 0; break;
		}
	}
	return 0;
}
int getPieceScoreExpert(Piece* piece){
	int sum=0;
	int x,y;
	if(piece->alive==true){
		x=piece->x;
		if(piece->color==WHITE){
			y=piece->y;
		}
		else if(piece->color==BLACK){
			y=GAME_N_ROWS-1-piece->y;
		}
		switch(piece->type){
			case('k'): sum+=20000;sum+=kingTable[y][x];break;
			case('q'): sum+= 900;sum+=queenTable[y][x];break;
			case('r'): sum+= 500;sum+=rookTable[y][x];break;
			case('b'): sum+= 330;sum+=bishopTable[y][x];break;
			case('n'): sum+= 320;sum+=knightTable[y][x];break;
			case('m'): sum+= 100;sum+=pawnTable[y][x];break;
		}
	}
	return sum;
}
bool isPieceAlive(Piece* piece){
	return piece->alive;
}
void movePiece(Piece* piece,int i,int j){//moves piece from x,y to i,j
	setPieceX(piece,i);
	setPieceY(piece,j);
}
char getPiecePrint(Piece* piece){
	if(piece==NULL){
		return EMPTY_PIECE;
	}
	if(piece->color==0){
		return (char)toupper(piece->type);
	}
	return piece->type;
}
char* getPieceName(Piece* piece){
	switch(piece->type){
		case('k'): if(piece->color==BLACK)return "BlackKing";else return "WhiteKing";break;
		case('r'): if(piece->color==BLACK)return "BlackRook";else return "WhiteRook";break;
		case('q'): if(piece->color==BLACK)return "BlackQueen";else return "WhiteQueen";break;
		case('b'): if(piece->color==BLACK)return "BlackBishop";else return "WhiteBishop";break;
		case('n'): if(piece->color==BLACK)return "BlackKnight";else return "WhiteKnight";break;
		case('m'): if(piece->color==BLACK)return "BlackPawn";else return "WhitePawn";break;
		default: NULL; break;
	}
	return NULL;
}
