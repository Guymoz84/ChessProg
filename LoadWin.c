 #include "LoadWin.h"

LoadWin* loadWindowCreate(){
	LoadWin* res = NULL;
	res=(LoadWin*) calloc(sizeof(LoadWin), sizeof(char));
	if (res==NULL){
		return NULL;
	}
	// Create an application window with the following LOAD:
	res->window = SDL_CreateWindow("ChessProg - Load", // window title
			SDL_WINDOWPOS_CENTERED,           // initial x position
			SDL_WINDOWPOS_CENTERED,           // initial y position
			400,                               // width, in pixels
			800,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
			);

	// Check that the window was successfully created
	if (res->window==NULL ) {
		loadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created loadWindow\n");
	res->renderer = SDL_CreateRenderer(res->window, -1,SDL_RENDERER_ACCELERATED);
	if (res->renderer==NULL ) {
		// In the case that the window could not be made...
		loadWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created loadRenderer\n");
	res->widgets= createLoadWindowWidgets(res->renderer);
	if (res->widgets==NULL ) {
		// In the case that the window could not be made...
		loadWindowDestroy(res);
		printf("Could not create widgets: %s\n", SDL_GetError());
		return NULL ;
	}
	printf("created loadWidgets\n");
	res->numOfWidgets=NUM_OF_WIDGETS_LOAD_WINDOW;
	res->first=true;
	res->file_num=1;
	char* path=getFilePath(res->file_num);
	res->legalload=legalPath(path);
	free(path);
	printf("created everything\n");
	return res;
}
//Helper function to create buttons in the simple window;
Widget** createLoadWindowWidgets(SDL_Renderer* renderer) {
	int i;
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = calloc(NUM_OF_WIDGETS_LOAD_WINDOW,sizeof(Widget*));
	if (widgets == NULL ) {
		return NULL ;
	}
	SDL_Rect slot1R = { .x =BUTTON_SHIFT+50, .y = 35, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot2R  = { .x =BUTTON_SHIFT+50, .y = 140, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot3R = { .x = BUTTON_SHIFT+50, .y = 245, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot4R = { .x = BUTTON_SHIFT+50, .y = 350, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect slot5R = { .x = BUTTON_SHIFT+50, .y = 455, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect backR = { .x = BUTTON_SHIFT+50, .y = 560, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	SDL_Rect loadR = { .x = BUTTON_SHIFT+50, .y = 665, .h = BUTTON_HEIGTH, .w = BUTTON_WIDTH };
	widgets[0] = createSimpleButton(renderer, &slot1R, "./gui/images/buttons/slot1.bmp", "./gui/images/buttons/slot1b.bmp",true);
	widgets[1] = createSimpleButton(renderer, &slot2R, "./gui/images/buttons/slot2.bmp","./gui/images/buttons/slot2b.bmp",true);
	widgets[2] = createSimpleButton(renderer, &slot3R, "./gui/images/buttons/slot3.bmp","./gui/images/buttons/slot3b.bmp",true);
	widgets[3] = createSimpleButton(renderer, &slot4R, "./gui/images/buttons/slot4.bmp","./gui/images/buttons/slot4b.bmp",true);
	widgets[4] = createSimpleButton(renderer, &slot5R, "./gui/images/buttons/slot5.bmp","./gui/images/buttons/slot5b.bmp",true);
	widgets[5] = createSimpleButton(renderer, &backR, "./gui/images/buttons/back.bmp",NULL,true);
	widgets[6] = createSimpleButton(renderer, &loadR, "./gui/images/buttons/load.bmp","./gui/images/buttons/loadb.bmp",true);
	for(i=0;i<NUM_OF_WIDGETS_LOAD_WINDOW;i++){
		if(widgets[i]==NULL){
			printf("widget number %d is broken",i);
			loadWindowWidgetsDestroy(widgets);
			break;
		}
	}
	return widgets;
}
void loadWindowDraw(LoadWin* src) {
	int i;
	if(src==NULL){
		return;
	}
	SDL_SetRenderDrawColor(src->renderer, 255, 255, 255, 255);
	SDL_RenderClear(src->renderer);
	((SimpleButton*) src->widgets[0]->data)->active=false;
	((SimpleButton*) src->widgets[1]->data)->active=false;
	((SimpleButton*) src->widgets[2]->data)->active=false;
	((SimpleButton*) src->widgets[3]->data)->active=false;
	((SimpleButton*) src->widgets[4]->data)->active=false;
	((SimpleButton*) src->widgets[src->file_num-1]->data)->active=true;
	((SimpleButton*) src->widgets[6]->data)->active=false;
	if(src->legalload==true){
		((SimpleButton*) src->widgets[6]->data)->active=true;
	}
	for (i=0; i < src->numOfWidgets; i++) {
		(src->widgets[i])->drawWidget(src->widgets[i]);
	}
	SDL_RenderPresent(src->renderer);
}
void loadWindowDestroy(LoadWin* src) {
	if (!src) {
		return;
	}
	loadWindowWidgetsDestroy(src->widgets);

	if (src->renderer != NULL ) {
		SDL_DestroyRenderer(src->renderer);
	}
	if (src->window != NULL ) {
		SDL_DestroyWindow(src->window);
	}
	free(src);
}
void loadWindowWidgetsDestroy(Widget** widgets){
	int i;
	for(i=0;i<NUM_OF_WIDGETS_LOAD_WINDOW;i++){
		if(widgets[i]!=NULL){
			destroyWidget(widgets[i]); //NULL SAFE
		}
	}
	printf("loadWindowWidgetsDestroy\n");
	if(widgets!=NULL){
		free(widgets);
	}
	printf("free widgets\n");
}
LOAD_EVENT loadWindowHandleEvent(LoadWin* src, SDL_Event* event) {
	char* path;
	if (!event) {
		return LOAD_EVENT_INVALID_ARGUMENT;
	}
	switch (event->type) {
	case SDL_MOUSEBUTTONUP:
		if (src->widgets[0]->handleEvent(src->widgets[0],event)==1) {
			src->file_num=1;
			path=getFilePath(src->file_num);
			src->legalload=legalPath(path);
			free(path);
			return LOAD_EVENT_UPDATE;
		} else if (src->widgets[1]->handleEvent(src->widgets[1],event)==1){
			src->file_num=2;
			path=getFilePath(src->file_num);
			src->legalload=legalPath(path);
			free(path);
			return LOAD_EVENT_UPDATE;
		}else if (src->widgets[2]->handleEvent(src->widgets[2],event)==1){
			src->file_num=3;
			path=getFilePath(src->file_num);
			src->legalload=legalPath(path);
			free(path);
			return LOAD_EVENT_UPDATE;
		}else if (src->widgets[3]->handleEvent(src->widgets[3],event)==1){
			src->file_num=4;
			path=getFilePath(src->file_num);
			src->legalload=legalPath(path);
			free(path);
			return LOAD_EVENT_UPDATE;
		}else if (src->widgets[4]->handleEvent(src->widgets[4],event)==1){
			src->file_num=5;
			path=getFilePath(src->file_num);
			src->legalload=legalPath(path);
			free(path);
			return LOAD_EVENT_UPDATE;
		}else if (src->widgets[5]->handleEvent(src->widgets[5],event)==1)  {
			return LOAD_EVENT_BACK;
		}else if (src->widgets[6]->handleEvent(src->widgets[6],event)==1){
			if(src->legalload==true){
				return LOAD_EVENT_START;
			}
			return LOAD_EVENT_UPDATE;
		}
		break;
	case SDL_WINDOWEVENT:
		if (event->window.event==SDL_WINDOWEVENT_CLOSE) {
			return LOAD_EVENT_QUIT;
		}
		break;
	default:
		return LOAD_EVENT_NONE;
	}
	return LOAD_EVENT_NONE;
}
char* getFilePath(int file_num){
	char* path=(char*)calloc(40,sizeof(char));
	char name[2],ending[5];
	strcpy(path,"./gui/files/");
	name[0]=(char)(file_num+48);
	name[1]='\0';
	strcpy(ending,".txt");
	strcat(path,name);
	strcat(path,ending);
	printf("the file path is: %s\n",path);
	return path;
}
bool legalPath(char* path){
	FILE* file=fopen(path,"r");
	if(file==NULL){
		printf("the file path is illegal\n");
		return false;
	}
	else{
		printf("the file path is legal\n");
		fclose(file);
		return true;
	}
}
