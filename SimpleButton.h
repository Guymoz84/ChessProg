#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Common.h"
#include "Widget.h" //A button is a widget also
#include <stdbool.h>
#define NUM_OF_WIDGETS_GAME_WINDOW 6
#define NUM_OF_WIDGETS_SETTINGS_WINDOW 12
#define NUM_OF_WIDGETS_LOAD_WINDOW 7
#define NUM_OF_WIDGETS_SAVE_WINDOW 3
#define NUM_OF_WIDGETS_MENU_WINDOW 3
#define BUTTON_WIDTH 240
#define BUTTON_HEIGTH 80
#define BUTTON_SHIFT 30

typedef struct simplebutton_t SimpleButton;
struct simplebutton_t{
	SDL_Texture* buttonTexture;
	SDL_Texture* buttonPressedTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool active;
	bool show;
};
/**
 *  Creates a new button for a window.
 *  @param - windowRender-the renderer for the new button,
 *  location-the location of the button(for presenting it on a window),
 *  image-the image of the new button,
 *  imagepressed-the image if the image is already pressed,
 *  show-if we would like to show the button at a window.
 *  @return
 *  An instant of the new button.
 */
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,const char* image,const char* imagepressed,bool show);//,const char* imagepressed
/**
 *  Destroys the button.
 *  @param-src - the button that needs to been destroyed.
 */
void destroySimpleButton(Widget* src);
/**
 * 	Handles all possible events for the button.
 *  @param-src - the button, event - the event that the user activated.
 *  @return
 *  1 at a legal event, else - 0.
 */
int handleSimpleButtonEvenet(Widget* src, SDL_Event* event);
/**
 *  Draws the button at the window.
 *  @param-src - the piece button that needs to been draw.
 */
void drawSimpleButton(Widget* src);

#endif
