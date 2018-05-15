#include "PieceButton.h"

Widget* createPieceButton(SDL_Renderer* windowRender, SDL_Rect* location,const char* image,bool show) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	PieceButton* data = (PieceButton*) malloc(sizeof(PieceButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (res == NULL || data == NULL || loadingSurface == NULL|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface); //Surface is not actually needed after texture is created
	data->buttonTexture = buttonTexture;
	data->location = copyRect(location);
	data->windowRenderer = windowRender;
	data->show=show;
	res->destroyWidget = destroyPieceButton;
	res->drawWidget = drawPieceButton;
	res->handleEvent = handlePieceButtonEvenet;
	res->data = data;
	return res;
}
void destroyPieceButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	PieceButton* castData = (PieceButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	free(castData);
	free(src);
}
void setPieceButtonTexture(Widget* src,const char* image){
	if (src == NULL) {
		return;
	}
	PieceButton* castData = (PieceButton*) src->data;
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(castData->windowRenderer,loadingSurface);
	if (loadingSurface == NULL|| buttonTexture == NULL) {
		SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
		SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
		return ;
	}
	SDL_FreeSurface(loadingSurface);
	castData->buttonTexture = buttonTexture;

}
int handlePieceButtonEvenet(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return 0; //Better to return an error value
	}
	PieceButton* castData = (PieceButton*) src->data;
	if(castData->show==false){
		return 0;
	}
	if (event->type == SDL_MOUSEMOTION||event->type == SDL_MOUSEBUTTONDOWN||event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}

void drawPieceButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	PieceButton* castData = (PieceButton*) src->data;
	if(castData->show==true){
		SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
	}
}
char* getPiecePath(char* piecename){
	char* path=(char*)calloc(50,sizeof(char));
	char name[15],ending[5];
	strcpy(path,"./gui/images/pieces/");
	strcpy(name,piecename);
	strcpy(ending,".bmp");
	strcat(path,name);
	strcat(path,ending);
	printf("the file path is: %s\n",path);
	return path;
}
