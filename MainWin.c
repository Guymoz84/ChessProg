#include "MainWin.h"

MainWin* mainWindowCreate() {
	MainWin* res = NULL;
	res = (MainWin*) calloc(sizeof(MainWin), sizeof(char));
	if (res == NULL ) {
		return NULL ;
	}

	// Create an application window with the following settings:
	res->mainWindow = SDL_CreateWindow("ChessProg - Main Menu", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			460,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->mainWindow == NULL ) {
		mainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created mainWindow\n");
	res->mainRenderer = SDL_CreateRenderer(res->mainWindow, -1,SDL_RENDERER_ACCELERATED);
	if (res->mainRenderer == NULL ) {
		// In the case that the window could not be made...
		mainWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created mainRenderer\n");
	res->widgets=createMainWindowWidgets(res->mainRenderer);
	if (res->widgets==NULL ) {
		// In the case that the window could not be made...
		mainWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created mainWidgets\n");
	res->numOfWidgets=NUM_OF_WIDGETS_MENU_WINDOW;
	res->first=true;
	printf("created everything\n");
	return res;
}
Widget** createMainWindowWidgets(SDL_Renderer* renderer) {
	int i;
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(NUM_OF_WIDGETS_MENU_WINDOW,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect newgameR = { .x =BUTTON_SHIFT+50, .y = 60, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect loadR = { .x = BUTTON_SHIFT+50, .y = 190, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect quitR = { .x = BUTTON_SHIFT+50, .y = 320, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[0] = createSimpleButton(renderer, &newgameR, "./gui/images/buttons/newgame.bmp",NULL,true);
	widgets[1] = createSimpleButton(renderer, &loadR, "./gui/images/buttons/load.bmp",NULL,true);
	widgets[2] = createSimpleButton(renderer, &quitR, "./gui/images/buttons/quit.bmp",NULL,true);
	for(i=0;i<NUM_OF_WIDGETS_MENU_WINDOW;i++){
		if(widgets[i]==NULL){
			printf("widget number %d is broken",i);
			mainWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}
void mainWindowDraw(MainWin* src) {
	int i;
	if(src==NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->mainRenderer, 255, 255, 255, 255);
	SDL_RenderClear(src->mainRenderer);
	for (i=0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->mainRenderer);
}
void mainWindowDestroy(MainWin* src) {
	if (!src) {
		return;
	}
	mainWindowWidgetsDestroy(src->widgets);
	if (src->mainRenderer != NULL ) {
		SDL_DestroyRenderer(src->mainRenderer);
	}
	if (src->mainWindow != NULL ) {
		SDL_DestroyWindow(src->mainWindow);
	}
	if(src!=NULL){
		free(src);
	}
}
void mainWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i=0;i<NUM_OF_WIDGETS_MENU_WINDOW;i++){
		if(widgets[i]!=NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	printf("mainWindowWidgetsDestroy\n");
	if(widgets!=NULL){
		free(widgets);
	}
	printf("free widgets\n");
}
MAIN_EVENT mainWindowHandleEvent(MainWin* src, SDL_Event* event) {
	if (!event) {
		return MAIN_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		printf("New game!\n");
		if (src->widgets[0]->handleEvent(src->widgets[0],event)==1) {
			printf("New game!");
			return MAIN_START;
		} else if (src->widgets[1]->handleEvent(src->widgets[1],event)==1) {
			return MAIN_LOAD;
		}else if (src->widgets[2]->handleEvent(src->widgets[2],event)==1){
			return MAIN_EXIT;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			return MAIN_EXIT;
		}
		break;
	default:
		return MAIN_NONE;
	}
	return MAIN_NONE;
}

void mainWindowHide(MainWin* src) {
	SDL_HideWindow(src->mainWindow);
}

void mainWindowShow(MainWin* src) {
	SDL_ShowWindow(src->mainWindow);
}
