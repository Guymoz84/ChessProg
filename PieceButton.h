#ifndef PIECEBUTTON_H_
#define PIECEBUTTON_H_

#include "Widget.h"
#include <stdbool.h>
#include "Common.h"
#include <stdio.h>

typedef struct piecebutton_t PieceButton;
struct piecebutton_t{
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	bool show;
};
/**
 *  Creates a new button for a piece.
 *  @param - windowRender-the renderer for the new button,
 *  location-the location of the button(for presenting it on the gameboard),
 *  image-the image of the new piece(as its type), show-if we would like to show the piece on the gameboard.
 *  @return
 *  An instant of the new button.
 */
Widget* createPieceButton(SDL_Renderer* windowRender, SDL_Rect* location,const char* image,bool show);
/**
 *  Destroys the piece button window.
 *  @param-src - the piece button that needs to been destroyed.
 */
void destroyPieceButton(Widget* src);
/**
 *  Sets a new texture for a piece.
 *  @param - src-the widget to set,
 *  image-the new image of the piece(as its type)
 */
void setPieceButtonTexture(Widget* src,const char* image);
/**
 * 	Handles all possible events for the piece.
 *  @param-src - the piece button, event - the event that the user activated.
 *  @return
 *  1 at a legal event, else - 0.
 */
int handlePieceButtonEvenet(Widget* src, SDL_Event* event);
/**
 *  Draws the piece button window.
 *  @param-src - the piece button that needs to been draw.
 */
void drawPieceButton(Widget* src);
/**
 * 	Creates a path to load from it the relavent picture.
 *  @param-piecename - the name of piece.
 *  @return
 *  a string - the picture path of the choosen piece.
 */
char* getPiecePath(char* piecename);
#endif /* PIECEBUTTON_H_ */
