#include "GameWin.h"

GameWin* gameWindowCreate(Game* game){
	GameWin* res=(GameWin*) calloc(sizeof(GameWin), sizeof(char));
	SDL_Surface* loadingSurface=NULL; //Used as temp surface
	if (res==NULL||game==NULL){
		printf("Couldn't create GameWin struct\n");
		return NULL ;
	}
	res->game=game;
	// Create an application window with the following settings:
	res->window=SDL_CreateWindow("ChessProg - Game", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			1020,                               // width, in pixels
			720,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);
	// Check that the window was successfully created
	if (res->window==NULL){
		printf("Could not create window: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	res->renderer=SDL_CreateRenderer(res->window, -1,SDL_RENDERER_ACCELERATED);
	if (res->renderer==NULL){
		printf("Could not create a renderer: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	//Create a background texture:
	loadingSurface=SDL_LoadBMP("./gui/images/chessboard.bmp");
	if (loadingSurface==NULL){
		printf("Could not create a surface: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	res->bgTexture=SDL_CreateTextureFromSurface(res->renderer,loadingSurface);
	if (res->bgTexture==NULL){
		printf("Could not create a texture: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //We finished with the surface -> delete it
	res=createGreenTexture(res,loadingSurface);
	res=createRedTexture(res,loadingSurface);
	res=createBlueTexture(res,loadingSurface);
	res->widgets=createGameWindowWidgets(res->renderer,res->game);
	if (res->widgets==NULL){
		gameWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created gameWidgets\n");
	res->numOfWidgets=NUM_OF_WIDGETS_GAME_WINDOW+game->user1->numpieces+game->user2->numpieces;
	res->first=true;
	res->saved=true;
	res->back_to_mainmenu=false;
	res->moves=NULL;
	return res;
}
GameWin* createGreenTexture(GameWin* res,SDL_Surface* loadingSurface){
	loadingSurface=SDL_LoadBMP("./gui/images/green.bmp");
	if (loadingSurface==NULL){
		printf("Could not create a green surface: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	res->greenTexture=SDL_CreateTextureFromSurface(res->renderer,loadingSurface);
	if (res->greenTexture==NULL){
		printf("Could not create a texture: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}
GameWin* createRedTexture(GameWin* res,SDL_Surface* loadingSurface){
	loadingSurface=SDL_LoadBMP("./gui/images/red.bmp");
	if (loadingSurface==NULL){
		printf("Could not create a red surface: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	res->redTexture=SDL_CreateTextureFromSurface(res->renderer,loadingSurface);
	if (res->redTexture==NULL){
		printf("Could not create a texture: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}
GameWin* createBlueTexture(GameWin* res,SDL_Surface* loadingSurface){
	loadingSurface=SDL_LoadBMP("./gui/images/blue.bmp");
	if (loadingSurface==NULL){
		printf("Could not create a blue surface: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	res->blueTexture=SDL_CreateTextureFromSurface(res->renderer,loadingSurface);
	if (res->blueTexture==NULL){
		printf("Could not create a texture: %s\n", SDL_GetError());
		gameWindowDestroy(res);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	return res;
}
Widget** createGameWindowWidgets(SDL_Renderer* renderer,Game* game){
	int i=0;
	if (renderer == NULL){
		return NULL ;
	}
	Widget** widgets=calloc(NUM_OF_WIDGETS_GAME_WINDOW+game->user1->numpieces+game->user2->numpieces,sizeof(Widget*));
	if (widgets == NULL){
		return NULL ;
	}
	SDL_Rect resetR ={.x =750,.y=30,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect saveR ={.x =750,.y=140,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect loadR ={.x=750,.y=250,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect undoR ={.x=750,.y=360,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect mainmenuR ={.x=750,.y=470,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	SDL_Rect quitR ={.x=750,.y=580,.h=BUTTON_HEIGTH,.w=BUTTON_WIDTH };
	widgets[0]=createSimpleButton(renderer, &resetR, "./gui/images/buttons/restart.bmp",NULL,true);
	widgets[1]=createSimpleButton(renderer, &saveR, "./gui/images/buttons/save.bmp",NULL,true);
	widgets[2]=createSimpleButton(renderer, &loadR, "./gui/images/buttons/load.bmp",NULL,true);
	widgets[3]=createSimpleButton(renderer, &undoR, "./gui/images/buttons/undo.bmp","./gui/images/buttons/undob.bmp",true);
	widgets[4]=createSimpleButton(renderer, &mainmenuR, "./gui/images/buttons/main.bmp",NULL,true);
	widgets[5]=createSimpleButton(renderer, &quitR, "./gui/images/buttons/quit.bmp",NULL,true);
	for(i=0;i<game->user1->numpieces;i++){
		if(game->user1->pieces[i]!=NULL){
			SDL_Rect pieceR ={.x=game->user1->pieces[i]->x*90,.y=game->user1->pieces[i]->y*90,.h=90,.w=90 };
			char* path=getPiecePath(getPieceName(game->user1->pieces[i]));
			widgets[i+NUM_OF_WIDGETS_GAME_WINDOW]=createPieceButton(renderer, &pieceR,path,game->user1->pieces[i]->alive);
			free(path);
		}
	}
	for(i=0;i<game->user2->numpieces;i++){
		if(game->user2->pieces[i]!=NULL){
			SDL_Rect pieceR ={.x=game->user2->pieces[i]->x*90,.y=game->user2->pieces[i]->y*90,.h=90,.w=90 };
			char* path=getPiecePath(getPieceName(game->user2->pieces[i]));
			widgets[i+NUM_OF_WIDGETS_GAME_WINDOW+game->user1->numpieces]=createPieceButton(renderer, &pieceR,path,game->user2->pieces[i]->alive);
			free(path);
		}
	}
	for(i=0;i<NUM_OF_WIDGETS_GAME_WINDOW+game->user1->numpieces+game->user2->numpieces;i++){
		if(widgets[i]==NULL){
			printf("widget number %d is broken",i);
			gameWindowWidgetsDestroy(widgets,game);
			break;
		}
	}
	return widgets;
}
void gameWindowDraw(GameWin* src,bool update){
	int numwidget,i,k;
	if(src==NULL){
		return;}
	SDL_Rect rec ={.x=0,.y=0,.w=720,.h=720 };
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	SDL_RenderCopy(src->renderer, src->bgTexture, NULL, &rec);
	if(update==true){//updates the locations according to the database
		drawUpdateGameWindow(src);
	}
	if(src->moves!=NULL){//draws get_moves
		for(k=0;k<src->moves->actualSize;k++){
			SDL_Rect recMoves ={.x=(src->moves->elements[k].i)*90,.y=(src->moves->elements[k].j)*90,.w=90,.h=90 };
			int num=checkIfEatMove(src->moves->elements[k]);
			gameSetMoveAux(src->game,&src->moves->elements[k]);
			if(gameThreat(src->game,src->moves->elements[k].i,src->moves->elements[k].j)!=NULL){//threat by opponent - red
				SDL_RenderCopy(src->renderer, src->redTexture, NULL, &recMoves);
			}
			else{
				if(num==2){//blue
					SDL_RenderCopy(src->renderer, src->blueTexture, NULL, &recMoves);
				}
				else{//non eat move and not threatened - green
					SDL_RenderCopy(src->renderer, src->greenTexture, NULL, &recMoves);
				}}
			undoLastMoveAux(src->game,&src->moves->elements[k]);
	}}
	for(i=0;i<NUM_OF_WIDGETS_GAME_WINDOW;i++){//draws the buttons
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	int lasttodraw=-1;
	for(i=0;i<src->game->user1->numpieces;i++){//draws the user1's pieces
		numwidget=i+NUM_OF_WIDGETS_GAME_WINDOW;
		if(src->game->user1->pieces[i]!=NULL){
			PieceButton* piecebutton=(PieceButton*)src->widgets[numwidget]->data;
			if(piecebutton->location->x!=src->game->user1->pieces[i]->x*90||piecebutton->location->y!=src->game->user1->pieces[i]->y*90){//if the location doesn't match the database
				lasttodraw=numwidget;//markering the last piece to draw
			}
			else{
				(src->widgets[numwidget])->drawWidget(src->widgets[numwidget]);
	}}}
	for(i=0;i<src->game->user2->numpieces;i++){//draws the user2's pieces
		numwidget=i+NUM_OF_WIDGETS_GAME_WINDOW+src->game->user1->numpieces;
		if(src->game->user2->pieces[i]!=NULL){
			PieceButton* piecebutton=(PieceButton*)src->widgets[numwidget]->data;
			if(piecebutton->location->x!=src->game->user2->pieces[i]->x*90||piecebutton->location->y!=src->game->user2->pieces[i]->y*90){//if the location doesn't match the database
				lasttodraw=numwidget;//markering the last piece to draw
			}
			else{
				(src->widgets[numwidget])->drawWidget(src->widgets[numwidget]);
	}}}
	if(lasttodraw!=-1){
		(src->widgets[lasttodraw])->drawWidget(src->widgets[lasttodraw]);//drawing the last piece
	}
	SDL_RenderPresent(src->renderer);
	arrayListDestroy(src->moves);//for get moves
	src->moves=NULL;//for get moves
}
void drawUpdateGameWindow(GameWin* src){
	int numwidget,i;
	((SimpleButton*) src->widgets[3]->data)->active=false;
	int historysize=arrayListSize(src->game->history);
	if(historysize!=0){
		if(src->game->settings.game_mode==ONE_PLAYER_MODE){
			((SimpleButton*) src->widgets[3]->data)->active=true;
		}
	}
	for(i=0;i<src->game->user1->numpieces;i++){
		numwidget=i+NUM_OF_WIDGETS_GAME_WINDOW;
		if(src->game->user1->pieces[i]!=NULL){
			((PieceButton*)src->widgets[numwidget]->data)->location->x=src->game->user1->pieces[i]->x*90;
			((PieceButton*)src->widgets[numwidget]->data)->location->y=src->game->user1->pieces[i]->y*90;
			if(src->game->user1->pieces[i]->alive==false){
				((PieceButton*)src->widgets[numwidget]->data)->show=false;
			}
			else if(src->game->user1->pieces[i]->alive==true){
				((PieceButton*)src->widgets[numwidget]->data)->show=true;
			}
		}
	}
	for(i=0;i<src->game->user2->numpieces;i++){
		numwidget=i+NUM_OF_WIDGETS_GAME_WINDOW+src->game->user1->numpieces;
		if(src->game->user2->pieces[i]!=NULL){
			if(src->game->user2->pieces[i]!=NULL){
				((PieceButton*)src->widgets[numwidget]->data)->location->x=src->game->user2->pieces[i]->x*90;
				((PieceButton*)src->widgets[numwidget]->data)->location->y=src->game->user2->pieces[i]->y*90;
				if(src->game->user2->pieces[i]->alive==false){
					((PieceButton*)src->widgets[numwidget]->data)->show=false;
				}
				else if(src->game->user2->pieces[i]->alive==true){
					((PieceButton*)src->widgets[numwidget]->data)->show=true;
				}
			}
		}
	}
}
void gameWindowDestroy(GameWin* src){
	if (!src){
		return;
	}
	gameWindowWidgetsDestroy(src->widgets,src->game);
	gameDestroy(src->game);
	if (src->bgTexture!=NULL){
		SDL_DestroyTexture(src->bgTexture);
	}
	if (src->greenTexture!=NULL){
		SDL_DestroyTexture(src->greenTexture);
	}
	if (src->redTexture!=NULL){
		SDL_DestroyTexture(src->redTexture);
	}
	if (src->blueTexture!=NULL){
		SDL_DestroyTexture(src->blueTexture);
	}
	if (src->renderer!=NULL){
		SDL_DestroyRenderer(src->renderer);
	}
	if (src->window!=NULL){
		SDL_DestroyWindow(src->window);
	}
	if(src!=NULL){
		free(src);
	}
}
void gameWindowWidgetsDestroy(Widget** widgets,Game* game){
	int i;
	for(i=0;i<NUM_OF_WIDGETS_GAME_WINDOW+game->user1->numpieces+game->user2->numpieces;i++){
		if(widgets[i]!=NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	printf("gameWindowWidgetsDestroy\n");
	if(widgets!=NULL){
		free(widgets);
	}
	printf("free widgets\n");
}
GAME_EVENT gameWindowHandleEvent(GameWin* src, SDL_Event* event){
	if (event==NULL||src==NULL){
		return GAME_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type){
	case SDL_MOUSEBUTTONUP:
		return buttonUp(src,event);
		break;
	case SDL_MOUSEBUTTONDOWN:{
		return buttonDown(src,event);
		break;
	}
	case SDL_WINDOWEVENT:
		if (event->window.event==SDL_WINDOWEVENT_CLOSE){
			return GAME_EVENT_EXIT;
		}
		break;
	default:
		return GAME_EVENT_NONE;
	}
	return GAME_EVENT_NONE;
}
GAME_EVENT buttonUp(GameWin* src,SDL_Event* event){
	if (src->widgets[0]->handleEvent(src->widgets[0],event)==1){
		gameWindowWidgetsDestroy(src->widgets,src->game);
		Game* tempgame=src->game;
		Game* newgame=gameCreate(HISTORY_SIZE);
		newgame=gameInitalize(newgame,src->game->settings);
		src->game=newgame;
		gameDestroy(tempgame);
		printf("Reset!\n");
		printGameBoard(src->game);
		src->saved=false;
		src->widgets=createGameWindowWidgets(src->renderer,src->game);
		if(src->game->settings.game_mode==ONE_PLAYER_MODE){
			if(src->game->currentPlayer==PLAYER_2_SYMBOL){
				computerMoveGui(src);
			}
		}
		return GAME_EVENT_UPDATE;
	}
	else if (src->widgets[1]->handleEvent(src->widgets[1],event)==1){
		src->saved=true;
		return GAME_EVENT_SAVE;
	}
	else if (src->widgets[2]->handleEvent(src->widgets[2],event)==1){
		src->saved=true;
		return GAME_EVENT_LOAD;
	}
	else if (src->widgets[3]->handleEvent(src->widgets[3],event)==1){
		if(src->game->settings.game_mode==TWO_PLAYERS_MODE){
			printf("Undo command not available in 2 players mode\n");
		}
		else if(src->game->settings.game_mode==ONE_PLAYER_MODE){
			undo(src->game);
			src->saved=false;
		}
		return GAME_EVENT_UPDATE;
	}
	else if (src->widgets[4]->handleEvent(src->widgets[4],event)==1){
		src->back_to_mainmenu=true;
		return GAME_EVENT_MAIN_MENU;
	}
	else if (src->widgets[5]->handleEvent(src->widgets[5],event)==1){
		printf("Exit from Game window\n");
		return GAME_EVENT_EXIT;
	}
	return GAME_EVENT_NONE;
}
GAME_EVENT buttonDown(GameWin* src,SDL_Event* event){
	bool rightclick=false;
	ArrayList* moves;
	int x,y,i,j,legal=-1;
	for(i=0;i<gameGetCurrentPlayer(src->game)->numpieces;i++){
		int numwidget=i+NUM_OF_WIDGETS_GAME_WINDOW;
		if(src->game->currentPlayer==PLAYER_2_SYMBOL)
			numwidget+=src->game->user1->numpieces;
		if (src->widgets[numwidget]->handleEvent(src->widgets[numwidget],event)==1){
			x=event->button.x/90;
			y=event->button.y/90;
			if(src->game->gameBoard[y][x]!=NULL){
				if(src->game->gameBoard[y][x]->player==src->game->currentPlayer){
					if(src->game->gameBoard[y][x]->alive==true){
						legal=numwidget;
						if(event->button.button==SDL_BUTTON_RIGHT&&src->game->settings.game_level<3){
							printf("Right button has been clicked!\n");
							if(src->game->settings.game_mode==ONE_PLAYER_MODE){
								rightclick=true;}}}}}}}
	if(rightclick==true){
		if(src->game->gameBoard[y][x]!=NULL&&src->game->gameBoard[y][x]->player==src->game->currentPlayer){
			moves=getMoves(src->game,src->game->gameBoard[y][x]);
			moves=sortForGetMoves(moves);
			src->moves=moves;}}
	else{
		if(legal!=-1){
			SDL_Event eventmotion;
			SDL_WaitEvent(&eventmotion);
			while(eventmotion.type==SDL_MOUSEMOTION){
				((PieceButton*)src->widgets[legal]->data)->location->x=eventmotion.motion.x-45;
				((PieceButton*)src->widgets[legal]->data)->location->y=eventmotion.motion.y-45;
				gameWindowDraw(src,false);
				SDL_WaitEvent(&eventmotion);
				if(eventmotion.type!=SDL_MOUSEMOTION){
					break;}}
			i=eventmotion.button.x/90;
			j=eventmotion.button.y/90;
			Move move;
			setMove(&move,x,y,i,j);
			bool validmove=gameIsValidMove(src->game,&move);
			if(validmove==true){
				src->saved=false;
				gameSetMove(src->game,move);
				gameWindowDraw(src,true);
				changeTurn(src->game);
				GAME_MESSAGE gameover=checkWinnerOrTie(src->game);
				if(src->game->settings.game_mode==TWO_PLAYERS_MODE){
					if(gameover==GAME_CHECK){
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","Check!", NULL );}
					else if(gameover==GAME_CHECKMATE||gameover==GAME_TIE){
						checkMateOrTie(src,gameover);
						return GAME_EVENT_MAIN_MENU;}}
				else if(src->game->settings.game_mode==ONE_PLAYER_MODE){
					if(gameover==GAME_CHECKMATE||gameover==GAME_TIE){
						checkMateOrTie(src,gameover);
						return GAME_EVENT_MAIN_MENU;}
					else if(gameover==GAME_CHECK){
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","Check!", NULL);}
					if(computerMoveGui(src)==GAME_EVENT_MAIN_MENU){
						return GAME_EVENT_MAIN_MENU;
					}}}}}
	return GAME_EVENT_UPDATE;}
GAME_EVENT computerMoveGui(GameWin* src){
	GAME_MESSAGE gameover;
	Move bestmove;
	setMove(&bestmove,0,0,1,1);//Initial
	bestmove=chooseBestMove(src->game);
	gameSetMove(src->game,bestmove);
	changeTurn(src->game);
	gameWindowDraw(src,true);
	gameover=checkWinnerOrTie(src->game);
	if(gameover==GAME_CHECK){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","Check!", NULL );
	}
	else if(gameover==GAME_CHECKMATE||gameover==GAME_TIE){
		checkMateOrTie(src,gameover);
		return GAME_EVENT_MAIN_MENU;
	}
	return GAME_EVENT_NONE;
}
void checkMateOrTie(GameWin* src,GAME_MESSAGE gameover){
	if(gameover==GAME_CHECKMATE){
		if(src->game->settings.game_mode==1){
			if(src->game->currentPlayer==PLAYER_1_SYMBOL){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","YOU HAVE LOST!", NULL );
			}
			else{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","YOU HAVE WON!", NULL );
			}
		}
		else{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","YOU HAVE LOST!", NULL );
		}
	}
	else if(gameover==GAME_TIE){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","IT'S A TIE!", NULL );
	}
	src->saved=true;
}
void gameWindowHide(GameWin* src){
	SDL_HideWindow(src->window);
}

void gameWindowShow(GameWin* src){
	SDL_ShowWindow(src->window);
}
