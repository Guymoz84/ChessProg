#include "GUIManager.h"

GuiManager* managerCreate(){
	GuiManager* res=(GuiManager*) malloc(sizeof(GuiManager));
	if(res==NULL){
		return NULL ;
	}
	res->mainWin=mainWindowCreate();
	if(res->mainWin==NULL){
		free(res);
		return NULL ;
	}
	res->settingsWin=NULL;
	res->loadWin=NULL;
	res->saveWin=NULL;
	res->gameWin=NULL;
	res->activeWin=MAIN_WINDOW_ACTIVE;
	return res;
}
void managerDestroy(GuiManager* src){
	if(src==NULL){
		return;
	}
	if(src->gameWin!=NULL){
		gameWindowDestroy(src->gameWin);
	}
	if(src->mainWin!=NULL){
		mainWindowDestroy(src->mainWin);
	}
}
void managerDraw(GuiManager* src){
	if(src==NULL){
		return;
	}
	switch(src->activeWin){
	case MAIN_WINDOW_ACTIVE:
		mainWindowDraw(src->mainWin);
		break;
	case SETTINGS_WINDOW_ACTIVE:
		settingsWindowDraw(src->settingsWin);
		break;
	case LOAD_WINDOW_ACTIVE:
		loadWindowDraw(src->loadWin);
		break;
	case SAVE_WINDOW_ACTIVE:
		saveWindowDraw(src->saveWin);
		break;
	case GAME_WINDOW_ACTIVE:
		gameWindowDraw(src->gameWin,true);
		break;
	}
}
MANAGER_EVENET handleManagerDueToMainEvent(GuiManager* src,MAIN_EVENT event){
	if(src==NULL){
		return MANAGER_NONE;
	}
	switch(event){
	case MAIN_START:
		mainWindowHide(src->mainWin);
		src->settingsWin=settingsWindowCreate();
		src->settingsWin->page=1;
		if(src->settingsWin==NULL){
			printf("Couldn't create settings window\n");
			return MANAGER_QUIT;
		}
		src->activeWin=SETTINGS_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	case MAIN_LOAD:
		mainWindowHide(src->mainWin);
		src->loadWin=loadWindowCreate();
		if(src->loadWin==NULL){
			printf("Couldn't create load window\n");
			return MANAGER_QUIT;
		}
		src->activeWin=LOAD_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	case MAIN_EXIT:
		return MANAGER_QUIT;
		break;
	default:
		return MANAGER_NONE;
	}
}
MANAGER_EVENET handleManagerDueToSettingsEvent(GuiManager* src,SETTINGS_EVENT event){
	if(src==NULL){
		return MANAGER_NONE;
	}
	Game* game;
	switch(event){
	case SETTINGS_EVENT_UPDATE:
		return MANAGER_UPDATE;
		break;
	case SETTINGS_EVENT_START:
		game=gameCreate(HISTORY_SIZE);
		game=gameInitalize(game,src->settingsWin->settings);
		src->gameWin=gameWindowCreate(game);
		src->activeWin=GAME_WINDOW_ACTIVE;
		settingsWindowDestroy(src->settingsWin);
		return MANAGER_UPDATE;
		break;
	case SETTINGS_EVENT_BACK:
		mainWindowShow(src->mainWin);
		src->activeWin=MAIN_WINDOW_ACTIVE;
		src->settingsWin->page=1;
		settingsWindowDestroy(src->settingsWin);
		return MANAGER_UPDATE;
		break;
	case SETTINGS_EVENT_QUIT:
		return MANAGER_QUIT;
		break;
	default:
		return MANAGER_NONE;
	}
}
MANAGER_EVENET handleManagerDueToLoadEvent(GuiManager* src,LOAD_EVENT event){
	if(src==NULL){
		return MANAGER_NONE;
	}
	Game* game;
	char* path;
	switch(event){
	case LOAD_EVENT_UPDATE:
		return MANAGER_UPDATE;
		break;
	case LOAD_EVENT_BACK:
		loadWindowDestroy(src->loadWin);
		if(src->gameWin!=NULL){
			gameWindowShow(src->gameWin);
			src->activeWin=GAME_WINDOW_ACTIVE;
		}
		else{
			mainWindowShow(src->mainWin);
			src->activeWin=MAIN_WINDOW_ACTIVE;
		}
		return MANAGER_UPDATE;
		break;
	case LOAD_EVENT_START:
		if(src->gameWin!=NULL){
			gameWindowDestroy(src->gameWin);
		}
		path=getFilePath(src->loadWin->file_num);
		game=load(path);
		free(path);
		loadWindowDestroy(src->loadWin);
		src->gameWin=gameWindowCreate(game);
		if(src->gameWin==NULL){
			printf("Couldn't create game window\n");
			return MANAGER_QUIT;
		}
		src->gameWin->saved=true;
		src->activeWin=GAME_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	case LOAD_EVENT_QUIT:
		return MANAGER_QUIT;
		break;
	default:
		return MANAGER_NONE;
	}
}
MANAGER_EVENET handleManagerDueToSaveEvent(GuiManager* src,SAVE_EVENT event){
	int i,ren;
	char *path,*newpath;
	if(src==NULL){
		return MANAGER_NONE;}
	switch(event){
	case SAVE_EVENT_YES:
		path=getFilePath(NUM_OF_SLOTS);
		if(legalPath(path)==true){
			remove(path);}
		free(path);
		for(i=NUM_OF_SLOTS-1;i>0;i--){
			path=getFilePath(i);
			if(legalPath(path)==true){//a file exist
				newpath=getFilePath(i+1);
				ren=rename(path,newpath);
				if(ren!=0){
					printf("fail to rename the path: %s\n",path);}
				free(newpath);}
			free(path);}
		path=getFilePath(1);
		save(src->gameWin->game,path);
		saveWindowDestroy(src->saveWin);
		free(path);
		if(src->gameWin->back_to_mainmenu==true){
			mainWindowShow(src->mainWin);
			src->activeWin=MAIN_WINDOW_ACTIVE;
			return MANAGER_UPDATE;}
		else{
			return MANAGER_QUIT;}
		break;
	case SAVE_EVENT_NO:
		saveWindowDestroy(src->saveWin);
		if(src->gameWin->back_to_mainmenu==true){
			printf("back to main menu\n");
			mainWindowShow(src->mainWin);
			src->activeWin=MAIN_WINDOW_ACTIVE;
			return MANAGER_UPDATE;}
		return MANAGER_QUIT;
		break;
	case SAVE_EVENT_CANCEL:
		saveWindowDestroy(src->saveWin);
		gameWindowShow(src->gameWin);
		src->activeWin=GAME_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	case SAVE_EVENT_QUIT:
		saveWindowDestroy(src->saveWin);
		gameWindowShow(src->gameWin);
		src->activeWin=GAME_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	default:
		return MANAGER_NONE;}
}
MANAGER_EVENET handleManagerDueToGameEvent(GuiManager* src,GAME_EVENT event){
	if(src==NULL){
		return MANAGER_NONE;
	}
	switch(event){
	case GAME_EVENT_UPDATE:
		return MANAGER_UPDATE;
		break;
	case GAME_EVENT_MAIN_MENU:
		if(src->gameWin->saved==true){
			printf("Game over or you haved saved the game\n");
			gameWindowDestroy(src->gameWin);
			src->gameWin=NULL;
			src->activeWin=MAIN_WINDOW_ACTIVE;
			mainWindowShow(src->mainWin);
			return MANAGER_UPDATE;
		}
		else{
			gameWindowHide(src->gameWin);
			src->saveWin=saveWindowCreate();
			if(src->saveWin==NULL){
				printf("Couldn't create save window\n");
				return MANAGER_QUIT;
			}
			src->activeWin=SAVE_WINDOW_ACTIVE;
			return MANAGER_UPDATE;
		}
		if(src->gameWin!=NULL){
			gameWindowHide(src->gameWin);
		}
		break;
	case GAME_EVENT_SAVE:
		saveGui(src);
		return MANAGER_UPDATE;
		break;
	case GAME_EVENT_LOAD:
		gameWindowHide(src->gameWin);
		src->loadWin=loadWindowCreate();
		if(src->loadWin==NULL){
			printf("Couldn't create load window\n");
			return MANAGER_QUIT;}
		src->activeWin=LOAD_WINDOW_ACTIVE;
		return MANAGER_UPDATE;
		break;
	case GAME_EVENT_EXIT:
		if(src->gameWin->saved==true){
			return MANAGER_QUIT;}
		else{
			gameWindowHide(src->gameWin);
			src->saveWin=saveWindowCreate();
			if(src->saveWin==NULL){
				printf("Couldn't create save window\n");
				return MANAGER_QUIT;}
			src->activeWin=SAVE_WINDOW_ACTIVE;
			return MANAGER_UPDATE;}
		break;
	default:
		return MANAGER_NONE;
	}
}
void saveGui(GuiManager* src){
	char *path,*newpath;
	int i=0,ren;
	path=getFilePath(NUM_OF_SLOTS);
	if(legalPath(path)==true){
		remove(path);
	}
	free(path);
	for(i=NUM_OF_SLOTS-1;i>0;i--){
		path=getFilePath(i);
		if(legalPath(path)==true){//a file exist
			newpath=getFilePath(i+1);
			ren=rename(path,newpath);
			if(ren!=0){
				printf("fail to rename the path: %s\n",path);}
			free(newpath);}
		free(path);}
	path=getFilePath(1);
	save(src->gameWin->game,path);
	free(path);
}
MANAGER_EVENET managerHandleEvent(GuiManager* src, SDL_Event* event){
	if(src==NULL||event==NULL){
		return MANAGER_NONE;}
	MAIN_EVENT mainEvent;
	SETTINGS_EVENT settingsEvent;
	LOAD_EVENT loadEvent;
	SAVE_EVENT saveEvent;
	GAME_EVENT gameEvent;
	switch(src->activeWin){
	case MAIN_WINDOW_ACTIVE:
		if(src->mainWin->first==true){
			src->mainWin->first=false;
			managerDraw(src);}
		mainEvent=mainWindowHandleEvent(src->mainWin, event);
		return handleManagerDueToMainEvent(src, mainEvent);
		break;
	case SETTINGS_WINDOW_ACTIVE:
		if(src->settingsWin->first==true){
			src->settingsWin->first=false;
			managerDraw(src);}
		settingsEvent=settingsWindowHandleEvent(src->settingsWin, event);
		return handleManagerDueToSettingsEvent(src, settingsEvent);
		break;
	case LOAD_WINDOW_ACTIVE:
		if(src->loadWin->first==true){
			src->loadWin->first=false;
			managerDraw(src);}
		loadEvent=loadWindowHandleEvent(src->loadWin, event);
		return handleManagerDueToLoadEvent(src, loadEvent);
		break;
	case SAVE_WINDOW_ACTIVE:
		saveEvent=saveWindowHandleEvent(src->saveWin, event);
		return handleManagerDueToSaveEvent(src, saveEvent);
		break;
	case GAME_WINDOW_ACTIVE:
		if(src->gameWin->first==true){
			src->gameWin->first=false;
			if(src->gameWin->game->settings.user_color==BLACK){
				src->gameWin->saved=false;
				src->gameWin->game=firstMoveComputer(src->gameWin->game);}
			managerDraw(src);
			GAME_MESSAGE gameover=checkWinnerOrTie(src->gameWin->game);
			if(gameover==GAME_CHECK){
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","Check!", NULL );}
			if(gameover==GAME_CHECKMATE){
				src->gameWin->saved=true;
				printf("checkmate\n");
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","YOU HAVE LOST!", NULL );
				return GAME_EVENT_MAIN_MENU;}
			if(gameover==GAME_TIE){
				src->gameWin->saved=true;
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Chessprog","IT'S A TIE!", NULL );
				return GAME_EVENT_MAIN_MENU;}}
		gameEvent=gameWindowHandleEvent(src->gameWin, event);
		return handleManagerDueToGameEvent(src, gameEvent);
		break;
	default:
		return MANAGER_NONE;}}
