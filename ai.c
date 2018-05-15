#include "ai.h"

int scoring(Game* game,char first){
	int user1sum=0,user2sum=0,i;
	if(game->settings.game_level<EXPERT_LEVEL){
		for(i=0;i<game->user1->numpieces;i++){//calculate the score of user1
			user1sum+=getPieceScore(game->user1->pieces[i]);
		}
		for(i=0;i<game->user2->numpieces;i++){//calculate the score of user2
			user2sum+=getPieceScore(game->user2->pieces[i]);
		}
	}
	else if(game->settings.game_level==EXPERT_LEVEL){
		return expertScoring(game,first);
	}
	return materialSum(user1sum,user2sum,first);
}
int expertScoring(Game* game,char first){
	int user1sum=0,user2sum=0,i;
	for(i=0;i<game->user1->numpieces;i++){//calculate the score of user1
		user1sum+=getPieceScoreExpert(game->user1->pieces[i]);
	}
	for(i=0;i<game->user2->numpieces;i++){//calculate the score of user2
		user2sum+=getPieceScoreExpert(game->user2->pieces[i]);
	}
	return materialSum(user1sum,user2sum,first);
}
int materialSum(int user1sum,int user2sum,char first){
	if(first==PLAYER_1_SYMBOL){
		return user1sum-user2sum;
	}
	else{
		return user2sum-user1sum;
	}
}
Move chooseBestMove(Game* game){
	int alpha=INT_MIN;
	int beta=INT_MAX;
	int i,j;
	int depth=game->settings.game_level;
	if(game->settings.game_level==EXPERT_LEVEL){
		depth=4;
	}
	User* current=gameGetCurrentPlayer(game);
	char first=current->symbol;
	Move bestmove;
	setMove(&bestmove,0,0,0,1);
	bestmove.eatmove=false;
	bestmove.eval=alpha;
	for(i=0;i<current->numpieces;i++){//for each piece
		Piece* piece =current->pieces[i];
		if(piece->alive==true){//if it's still alive
			ArrayList* moves=getMoves(game,piece);//calculates the available moves for the piece
			int numberofmoves=arrayListSize(moves);
			for(j=0;j<numberofmoves;j++){// goes through the available moves
				Move childmove=arrayListGetAt(moves,j);//the j's move
				gameSetMoveAux(game,&childmove);// sets the move
				changeTurn(game);
				childmove.eval=bestMoveRec(game,bestmove.eval,beta,0,first,depth-1);//recursive call
				changeTurn(game);
				undoLastMoveAux(game,&childmove);// undo the move
				if(childmove.eval>bestmove.eval){
					bestmove=childmove;
				}
			}
			arrayListDestroy(moves);
		}
	}
	return bestmove;
}
int bestMoveRec(Game* game,int alpha,int beta,int max_player,char first,int depth){
	User* current=gameGetCurrentPlayer(game);
	int moveval,i,j,score;
	GAME_MESSAGE message=checkWinnerOrTie(game);
	if(message==GAME_CHECKMATE){
		score=scoring(game,first);
		current->king->alive=true;
		return score;
	}
	else if(message==GAME_TIE||depth==0){
		return scoring(game,first);
	}
	else if(max_player==1){
		moveval=alpha;
		for(i=0;i<current->numpieces;i++){//for each piece
			Piece* piece =current->pieces[i];
			if(piece->alive==true){//if it's still alive
				ArrayList* moves=getMoves(game,piece);//calculates the available moves for the piece
				int numberofmoves=arrayListSize(moves);
				for(j=0;j<numberofmoves;j++){// goes through the available moves
					Move childmove=arrayListGetAt(moves,j);//the j's move
					gameSetMoveAux(game,&childmove);// sets the move
					changeTurn(game);
					childmove.eval=bestMoveRec(game,moveval,beta,0,first,depth-1);//recursive call
					changeTurn(game);
					undoLastMoveAux(game,&childmove);// undo the move
					moveval=max(moveval,childmove.eval);// return the max between the eval fields
					if(beta<=moveval){
						arrayListDestroy(moves);
						return moveval;
					}
				}
				arrayListDestroy(moves);
			}
		}
	}
	else{
		moveval=beta;
		for(i=0;i<current->numpieces;i++){
			Piece* piece =current->pieces[i];
			if(piece->alive==true){
				ArrayList* moves=getMoves(game,piece);
				int numberofmoves=arrayListSize(moves);
				for(j=0;j<numberofmoves;j++){
					Move childmove=arrayListGetAt(moves,j);
					gameSetMoveAux(game,&childmove);
					changeTurn(game);
					childmove.eval=bestMoveRec(game,alpha,moveval,1,first,depth-1);
					changeTurn(game);
					undoLastMoveAux(game,&childmove);
					moveval=min(moveval,childmove.eval);
					if(moveval<=alpha){
						arrayListDestroy(moves);
						return moveval;
					}
				}
				arrayListDestroy(moves);
			}
		}}
	return moveval;}
int max(int x,int y){
	if(x>y){
		return x;
	}
	return y;
}
int min(int x, int y){
	if(x<y){
		return x;
	}
	return y;
}

