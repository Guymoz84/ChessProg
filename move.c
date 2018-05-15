#include "move.h"
int setMove(Move* move,int x,int y,int i,int j){
	setMoveX(move,x);
	setMoveY(move,y);
	setMoveI(move,i);
	setMoveJ(move,j);
	return 1;
}
int setMoveX(Move* move,int x){
	move->x=x;
	return 1;
}
int setMoveY(Move* move,int y){
	move->y=y;
	return 1;
}
int setMoveI(Move* move,int i){
	move->i=i;
	return 1;
}
int setMoveJ(Move* move,int j){
	move->j=j;
	return 1;
}
int checkIfEatMove(Move move){//as the instructions - state 2 or a non eaten and non threatened move=0
	if(move.eatmove==true){//captures an opponent's piece and the move is threatened by the opponent
		return 2;
	}
	return 0;//regular move
}
