 #include "SettingsWin.h"

SettingsWin* settingsWindowCreate(){
	SettingsWin* res = NULL;
	res=(SettingsWin*) calloc(sizeof(SettingsWin), sizeof(char));
	if(res==NULL){
		return NULL;
	}
	// Create an application window with the following settings:
	res->window = SDL_CreateWindow("ChessProg - Settings", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			800,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);
	// Check that the window was successfully created
	if (res->window==NULL ) {
		settingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created settingsWindow\n");
	res->renderer = SDL_CreateRenderer(res->window, -1,SDL_RENDERER_ACCELERATED);
	if (res->renderer==NULL ) {
		// In the case that the window could not be made...
		settingsWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created settingsRenderer\n");
	res->widgets=createSettingsWindowWidgets(res->renderer);
	if (res->widgets==NULL ) {
		// In the case that the window could not be made...
		settingsWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created settingsWidgets\n");
	res->numOfWidgets=NUM_OF_WIDGETS_SETTINGS_WINDOW;
	res->first=true;
	printf("created everything\n");
	res->settings=createDefaultSettings();
	return res;
}
Widget** createSettingsWindowWidgets(SDL_Renderer* renderer) {
	int i;
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(NUM_OF_WIDGETS_SETTINGS_WINDOW,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}
	//page 1
	SDL_Rect oneplayerR = { .x =BUTTON_SHIFT+50, .y = 30, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect twoplayersR = { .x =BUTTON_SHIFT+50, .y = 140, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	//page 2
	SDL_Rect noobR = { .x = BUTTON_SHIFT+50, .y = 30, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect easyR = { .x = BUTTON_SHIFT+50, .y = 140, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect moderateR = { .x = BUTTON_SHIFT+50, .y = 250, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect hardR = { .x = BUTTON_SHIFT+50, .y = 360, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect expertR = { .x = BUTTON_SHIFT+50, .y = 470, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	//page 3
	SDL_Rect blackR = { .x = BUTTON_SHIFT+50, .y = 50, .h = 90, .w = 90 };
	SDL_Rect whiteR = { .x = BUTTON_SHIFT+200, .y = 50, .h = 90, .w = 90 };
	//common
	SDL_Rect nextR = { .x = BUTTON_SHIFT+50, .y = 580, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect startR = { .x = BUTTON_SHIFT+50, .y = 580, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect backR = { .x = BUTTON_SHIFT+50, .y = 690, .h =BUTTON_HEIGTH, .w = BUTTON_WIDTH };

	widgets[0] = createSimpleButton(renderer, &oneplayerR,"./gui/images/buttons/oneplayer.bmp","./gui/images/buttons/oneplayerb.bmp",false);
	widgets[1] = createSimpleButton(renderer, &twoplayersR, "./gui/images/buttons/twoplayers.bmp","./gui/images/buttons/twoplayersb.bmp",false);
	widgets[2] = createSimpleButton(renderer, &nextR, "./gui/images/buttons/next.bmp",NULL,false);
	widgets[3] = createSimpleButton(renderer, &startR, "./gui/images/buttons/start.bmp",NULL,false);
	widgets[4] = createSimpleButton(renderer, &backR, "./gui/images/buttons/back.bmp",NULL,false);
	widgets[5] = createSimpleButton(renderer, &noobR, "./gui/images/buttons/noob.bmp","./gui/images/buttons/noobb.bmp",false);
	widgets[6] = createSimpleButton(renderer, &easyR, "./gui/images/buttons/easy.bmp","./gui/images/buttons/easyb.bmp",false);
	widgets[7] = createSimpleButton(renderer, &moderateR, "./gui/images/buttons/moderate.bmp","./gui/images/buttons/moderateb.bmp",false);
	widgets[8] = createSimpleButton(renderer, &hardR, "./gui/images/buttons/hard.bmp","./gui/images/buttons/hardb.bmp",false);
	widgets[9] = createSimpleButton(renderer, &expertR, "./gui/images/buttons/expert.bmp","./gui/images/buttons/expertb.bmp",false);
	widgets[10] = createSimpleButton(renderer, &blackR, "./gui/images/pieces/BlackKingb.bmp","./gui/images/pieces/BlackKing.bmp",false);//don't touchs
	widgets[11] = createSimpleButton(renderer, &whiteR, "./gui/images/pieces/WhiteKingb.bmp","./gui/images/pieces/WhiteKing.bmp",false);//don't touch

	for(i=0;i<NUM_OF_WIDGETS_SETTINGS_WINDOW;i++){
		if(widgets[i]==NULL){
			printf("widget number %d is broken",i);
			settingsWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}
void settingsWindowDraw(SettingsWin* src) {
	int i;
	if(src==NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	for (i=0; i < src->numOfWidgets; i++) {
		((SimpleButton*)src->widgets[i]->data)->show=false;
		((SimpleButton*)src->widgets[i]->data)->active=false;
	}
	switch(src->page){
		case(1):{
			((SimpleButton*) src->widgets[0]->data)->show=true;
			((SimpleButton*) src->widgets[1]->data)->show=true;
			if(src->settings.game_mode==ONE_PLAYER_MODE){
				((SimpleButton*) src->widgets[0]->data)->active=true;
				((SimpleButton*) src->widgets[2]->data)->show=true;
			}
			else if(src->settings.game_mode==TWO_PLAYERS_MODE){
				((SimpleButton*) src->widgets[1]->data)->active=true;
				((SimpleButton*) src->widgets[3]->data)->show=true;
				((SimpleButton*) src->widgets[3]->data)->active=true;
			}
		}break;
		case(2):{
			((SimpleButton*) src->widgets[5]->data)->show=true;
			((SimpleButton*) src->widgets[6]->data)->show=true;
			((SimpleButton*) src->widgets[7]->data)->show=true;
			((SimpleButton*) src->widgets[8]->data)->show=true;
			((SimpleButton*) src->widgets[9]->data)->show=true;
			((SimpleButton*) src->widgets[src->settings.game_level+4]->data)->active=true;
			((SimpleButton*) src->widgets[2]->data)->show=true;
		}break;
		case(3):{
			((SimpleButton*) src->widgets[10]->data)->show=true;
			((SimpleButton*) src->widgets[11]->data)->show=true;
			((SimpleButton*) src->widgets[src->settings.user_color+10]->data)->active=true;
			((SimpleButton*) src->widgets[3]->data)->show=true;
			((SimpleButton*) src->widgets[3]->data)->active=true;
		}break;
	}
	((SimpleButton*) src->widgets[4]->data)->show=true;
	for (i=0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->renderer);
}
void settingsWindowDestroy(SettingsWin* src) {
	if (!src) {
		return;
	}
	settingsWindowWidgetsDestroy(src->widgets);
	if (src->renderer != NULL ) {
		SDL_DestroyRenderer(src->renderer);
	}
	if (src->window != NULL ) {
		SDL_DestroyWindow(src->window);
	}
	if(src!=NULL){
		free(src);
	}
}
void settingsWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i=0;i<NUM_OF_WIDGETS_SETTINGS_WINDOW;i++){
		if(widgets[i]!=NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	printf("settingsWindowWidgetsDestroy\n");
	if(widgets!=NULL){
		free(widgets);
	}
	printf("free widgets\n");
}
SETTINGS_EVENT settingsWindowHandleEvent(SettingsWin* src, SDL_Event* event) {
	if (!event) {
		return SETTINGS_EVENT_INVALID_ARGUMENT;}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (src->widgets[0]->handleEvent(src->widgets[0],event)==1){
			src->settings.game_mode=ONE_PLAYER_MODE;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[1]->handleEvent(src->widgets[1],event)==1){
			src->settings.game_mode=TWO_PLAYERS_MODE;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[2]->handleEvent(src->widgets[2],event)==1){
			src->page++;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[3]->handleEvent(src->widgets[3],event)==1){
			return SETTINGS_EVENT_START;
		}
		else if (src->widgets[4]->handleEvent(src->widgets[4],event)==1){
			if(src->page==1){
				return SETTINGS_EVENT_BACK;
			}
			src->page--;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[5]->handleEvent(src->widgets[5],event)==1){
			src->settings.game_level=1;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[6]->handleEvent(src->widgets[6],event)==1){
			src->settings.game_level=2;
			return SETTINGS_EVENT_UPDATE;
		}
		else if (src->widgets[7]->handleEvent(src->widgets[7],event)==1){
			src->settings.game_level=3;
			return SETTINGS_EVENT_UPDATE;}
		else if (src->widgets[8]->handleEvent(src->widgets[8],event)==1){
			src->settings.game_level=4;
			return SETTINGS_EVENT_UPDATE;}
		else if (src->widgets[9]->handleEvent(src->widgets[9],event)==1){
			src->settings.game_level=5;
			return SETTINGS_EVENT_UPDATE;}
		else if (src->widgets[10]->handleEvent(src->widgets[10],event)==1){
			src->settings.user_color=BLACK;
			return SETTINGS_EVENT_UPDATE;}
		else if (src->widgets[11]->handleEvent(src->widgets[11],event)==1){
			src->settings.user_color=WHITE;
			return SETTINGS_EVENT_UPDATE;}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event==SDL_WINDOWEVENT_CLOSE) {
			return SETTINGS_EVENT_QUIT;}
		break;
	default:
		return SETTINGS_EVENT_NONE;}
	return SETTINGS_EVENT_NONE;
}
void settingsWindowHide(SettingsWin* src) {
	SDL_HideWindow(src->window);
}

void settingsWindowShow(SettingsWin* src) {
	SDL_ShowWindow(src->window);
}
