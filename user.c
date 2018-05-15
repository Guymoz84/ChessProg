#include "user.h"
User* userCreate(char symbol,int color){
	User* user=(User*)malloc(sizeof(User));
	if(!user){
		free(user);
		exit(0);
	}
	setUserColor(user,color);
	setUserSymbol(user,symbol);
	user->numpieces=0;
	return user;
}
User* userInitalize(User* user){
	int j;
	user->numpieces=NUM_PIECES;
	//inserting the pieces by the order of their value- king=100,queen=9,rook=5,bishop=3,knight=3,pawn=1
	if(user->color==BLACK){
		user->pieces[0]=createPiece(4,0,user->symbol,'k',user->color);
		user->pieces[1]=createPiece(3,0,user->symbol,'q',user->color);
		//rook setup
		user->pieces[2]=createPiece(0,0,user->symbol,'r',user->color);
		user->pieces[3]=createPiece(7,0,user->symbol,'r',user->color);
		//bishop setup
		user->pieces[4]=createPiece(2,0,user->symbol,'b',user->color);
		user->pieces[5]=createPiece(5,0,user->symbol,'b',user->color);
		//knight setup
		user->pieces[6]=createPiece(1,0,user->symbol,'n',user->color);
		user->pieces[7]=createPiece(6,0,user->symbol,'n',user->color);
		//pawn setup
		for(j=GAME_N_ROWS;j<user->numpieces;j++){
			user->pieces[j]=createPiece(j-GAME_N_ROWS,1,user->symbol,'m',user->color);
		}
	}
	else if(user->color==WHITE){
		user->pieces[0]=createPiece(4,7,user->symbol,'k',user->color);
		user->pieces[1]=createPiece(3,7,user->symbol,'q',user->color);
		//rook setup
		user->pieces[2]=createPiece(0,7,user->symbol,'r',user->color);
		user->pieces[3]=createPiece(7,7,user->symbol,'r',user->color);
		//bishop setup
		user->pieces[4]=createPiece(2,7,user->symbol,'b',user->color);
		user->pieces[5]=createPiece(5,7,user->symbol,'b',user->color);
		//knight setup
		user->pieces[6]=createPiece(1,7,user->symbol,'n',user->color);
		user->pieces[7]=createPiece(6,7,user->symbol,'n',user->color);
		//pawn setup
		for(j=GAME_N_ROWS;j<user->numpieces;j++){
			user->pieces[j]=createPiece(j-GAME_N_ROWS,6,user->symbol,'m',user->color);
		}
	}
	user->king=user->pieces[0];
	return user;
}
User* userCopy(User* user){
	User* copy=userCreate(user->symbol,user->color);
	int i;
	for(i=0;i<user->numpieces;i++){
		copy->pieces[i]=pieceCopy(user->pieces[i]);
		if(copy->pieces[i]->type=='k'){
			copy->king=copy->pieces[i];
		}
	}
	copy->numpieces=user->numpieces;
	return copy;
}
void userDestroy(User* user){
	int i;
	for(i=0;i<user->numpieces;i++){
		destroyPiece(user->pieces[i]);
	}
	free(user);
}
int setUserColor(User* user,int color){
	user->color=color;
	return 1;
}
int setUserSymbol(User* user,char symbol){
	user->symbol=symbol;
	return 1;
}
Piece* addUserPiece(User* user,int x,int y,char type){
	user->pieces[user->numpieces]=createPiece(x,y,user->symbol,type,user->color);
	user->numpieces++;
	return user->pieces[user->numpieces-1];
}
