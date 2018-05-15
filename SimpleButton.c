#include <stdlib.h>
#include <stdio.h>
#include "SimpleButton.h"
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,const char* image,const char* imagepressed,bool show){
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SimpleButton* data = (SimpleButton*) malloc(sizeof(SimpleButton));
	SDL_Surface* loadingSurface = SDL_LoadBMP(image); //We use the surface as a temp var;
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if(imagepressed==NULL){
		data->buttonPressedTexture = NULL;
		data->active=true;
	}
	else{
		SDL_Surface* loadingPressedSurface = SDL_LoadBMP(imagepressed); //We use the surface as a temp var;
		SDL_Texture* buttonPressedTexture = SDL_CreateTextureFromSurface(windowRender,loadingPressedSurface);
		SDL_FreeSurface(loadingPressedSurface);
		data->buttonPressedTexture = buttonPressedTexture;
		data->active=false;
		printf("load a pressed image\n");
		if(buttonPressedTexture==NULL){
			free(res);
			free(data);
			SDL_FreeSurface(loadingSurface); //It is safe to pass NULL
			SDL_DestroyTexture(buttonTexture); ////It is safe to pass NULL
			return NULL ;
		}
	}
	if (res==NULL||data==NULL||loadingSurface==NULL||buttonTexture==NULL){
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
	res->destroyWidget = destroySimpleButton;
	res->drawWidget = drawSimpleButton;
	res->handleEvent = handleSimpleButtonEvenet;
	res->data = data;
	return res;
}
void destroySimpleButton(Widget* src) {
	if (src==NULL){
		return;
	}
	SimpleButton* castData =(SimpleButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	SDL_DestroyTexture(castData->buttonPressedTexture);
	free(castData);
	free(src);
}

int handleSimpleButtonEvenet(Widget* src, SDL_Event* event) {
	if (src == NULL || event == NULL) {
		return 0;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if(castData->show==false){
		return 0;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			return 1;
		}
	}
	return 0;
}

void drawSimpleButton(Widget* src) {
	if (src==NULL){
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if(castData->show==true){
		if(castData->buttonPressedTexture==NULL){
			SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
		}
		else{
			if(castData->active==true){
				SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,	castData->location);
			}
			else{
				SDL_RenderCopy(castData->windowRenderer, castData->buttonPressedTexture, NULL,	castData->location);
			}
		}

	}
}
