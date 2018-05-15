#ifndef SAVEWIN_H_
#define SAVEWIN_H_
#include "GameWin.h"

typedef enum {
	SAVE_EVENT_YES,
	SAVE_EVENT_NO,
	SAVE_EVENT_CANCEL,
	SAVE_EVENT_UPDATE,
	SAVE_EVENT_NONE,
	SAVE_EVENT_QUIT,
	SAVE_EVENT_INVALID_ARGUMENT,
} SAVE_EVENT;

typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	Widget** widgets;
	int numOfWidgets;
	bool legalsave;
}SaveWin;
/**
 *  Creates a new window for the save option when
 *  exiting the game without saveing the current game status.
 *  @return
 *  An instant of the new save window.
 */
SaveWin* saveWindowCreate();
/**
 *  Creates new widgets for the save window.
 *  @param-renderer - the renderer for the window.
 *  @return
 *  All widgets for the save window
 */
Widget** createSaveWindowWidgets(SDL_Renderer* renderer);
/**
 *  Draws the current save window.
 *  @param-src - the save window that needs to been draw.
 */
void saveWindowDraw(SaveWin* src);
/**
 *  Destroys the save window, uses the destroy widgets function
 *  also(that destroys all the widgets of the save window).
 *  @param-src - the save window that needs to been destroyed.
 */
void saveWindowDestroy(SaveWin* src);
void saveWindowWidgetsDestroy(Widget** widgets);
/**
 * 	Handles all possible events, depending if the user chooses to save the game or not.
 *  @return
 *  a SAVE_EVENT - depends on choice of the user.
 */
SAVE_EVENT saveWindowHandleEvent(SaveWin* src, SDL_Event* event);

#endif /* SAVEWIN_H_ */
