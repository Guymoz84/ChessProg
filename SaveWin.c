 #include "SaveWin.h"

SaveWin* saveWindowCreate(){
	SaveWin* res = NULL;
	res=(SaveWin*) calloc(sizeof(SaveWin), sizeof(char));
	if (res==NULL){
		return NULL;
	}

	// Create an application window with the following SAVE:
	res->window = SDL_CreateWindow("Save The Game?", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			400,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->window==NULL ) {
		saveWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created saveWindow\n");
	res->renderer = SDL_CreateRenderer(res->window, -1,SDL_RENDERER_ACCELERATED);
	if (res->renderer==NULL ) {
		// In the case that the window could not be made...
		saveWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created saveRenderer\n");
	res->widgets= createSaveWindowWidgets(res->renderer);
	if (res->widgets==NULL ) {
		// In the case that the window could not be made...
		saveWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created saveWidgets\n");
	res->numOfWidgets=NUM_OF_WIDGETS_SAVE_WINDOW;
	res->legalsave=false;
	printf("created everything in save window\n");
	return res;
}
//Helper function to create buttons in the simple window;
Widget** createSaveWindowWidgets(SDL_Renderer* renderer) {
	int i;
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(NUM_OF_WIDGETS_SAVE_WINDOW,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect yesR = { .x =BUTTON_SHIFT+50, .y = 30, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect noR  = { .x =BUTTON_SHIFT+50, .y = 150, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect cancelR = { .x = BUTTON_SHIFT+50, .y = 270, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[0] = createSimpleButton(renderer, &yesR, "./gui/images/buttons/yes.bmp",NULL,true);
	widgets[1] = createSimpleButton(renderer, &noR, "./gui/images/buttons/no.bmp",NULL,true);
	widgets[2] = createSimpleButton(renderer, &cancelR, "./gui/images/buttons/cancel.bmp",NULL,true);
	for(i=0;i<NUM_OF_WIDGETS_SAVE_WINDOW;i++){
		if(widgets[i]==NULL){
			printf("widget number %d is broken",i);
			saveWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}
void saveWindowDraw(SaveWin* src) {
	int i;
	if(src==NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	((SimpleButton*) src->widgets[0]->data)->active=true;
	((SimpleButton*) src->widgets[1]->data)->active=true;
	((SimpleButton*) src->widgets[2]->data)->active=true;
	for (i=0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->renderer);
}
void saveWindowDestroy(SaveWin* src) {
	if (!src) {
		return;
	}
	saveWindowWidgetsDestroy(src->widgets);

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
void saveWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i=0;i<NUM_OF_WIDGETS_SAVE_WINDOW;i++){
		if(widgets[i]!=NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	printf("saveWindowWidgetsDestroy\n");
	if(widgets!=NULL){
		free(widgets);
	}
	printf("free widgets\n");
}
SAVE_EVENT saveWindowHandleEvent(SaveWin* src, SDL_Event* event) {
	if (!event) {
		return SAVE_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (src->widgets[0]->handleEvent(src->widgets[0],event)==1) {
			src->legalsave=true;
			return SAVE_EVENT_YES;
		} else if (src->widgets[1]->handleEvent(src->widgets[1],event)==1){
			src->legalsave=false;
			return SAVE_EVENT_NO;
		}else if (src->widgets[2]->handleEvent(src->widgets[2],event)==1){
			return SAVE_EVENT_CANCEL;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event==SDL_WINDOWEVENT_CLOSE) {
			return SAVE_EVENT_QUIT;
		}
		break;
	default:
		return SAVE_EVENT_NONE;
	}
	return SAVE_EVENT_NONE;
}
