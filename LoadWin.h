#ifndef LOADWIN_H_
#define LOADWIN_H_

#include <stdio.h>
#include "SimpleButton.h"

typedef enum {
	LOAD_EVENT_SLOT_1,
	LOAD_EVENT_SLOT_2,
	LOAD_EVENT_SLOT_3,
	LOAD_EVENT_SLOT_4,
	LOAD_EVENT_SLOT_5,
	LOAD_EVENT_QUIT,
	LOAD_EVENT_BACK,
	LOAD_EVENT_START,
	LOAD_EVENT_INVALID_ARGUMENT,
	LOAD_EVENT_NONE,
	LOAD_EVENT_UPDATE,
} LOAD_EVENT;

typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
	int file_num;
	bool legalload;
}LoadWin;
/**
 *  Creates a new window for the load option.
 *  @return
 *  An instant of the new load window.
 */
LoadWin* loadWindowCreate();
/**
 *  Creates new widgets for the load window.
 *  @param-renderer - the renderer for the window.
 *  @return
 *  All widgets for the load window
 */
Widget** createLoadWindowWidgets(SDL_Renderer* renderer);
/**
 *  Draws the current load window.
 *  @param-src - the load window that needs to been draw.
 */
void loadWindowDraw(LoadWin* src);
/**
 *  Destroys the load window, uses the destroy widgets function
 *  also(that destroys all the widgets of the load window).
 *  @param-src - the load window that needs to been destroyed.
 */
void loadWindowDestroy(LoadWin* src);
void loadWindowWidgetsDestroy(Widget** widgets);
/**
 * 	Handles all possible events, depending the slot that is possible to load.
 *  @param-src - the load window, event - the event that the user activated.
 *  @return
 *  a LOAD_EVENT - depends on the slot that the user chooses.
 */
LOAD_EVENT loadWindowHandleEvent(LoadWin* src, SDL_Event* event);
/**
 * 	Creates a path to load from it the relavent slot.
 *  @param-file_num - the number of slot that has been choosen from the user.
 *  @return
 *  a string - the file path of the choosen slot.
 */
char* getFilePath(int file_num);
/**
 * 	Checks if the given path exists or not.
 *  @param-path of a file(or doesn't exists).
 *  @return
 *  true if the path exists, else-false.
 */
bool legalPath(char* path);

#endif /* LOADWIN_H_ */
