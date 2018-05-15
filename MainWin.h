#ifndef MAINWIN_H_
#define MAINWIN_H_
#include "SimpleButton.h"
#include <stdio.h>
typedef enum {
	MAIN_EXIT,MAIN_LOAD, MAIN_START, MAIN_INVALID_ARGUMENT, MAIN_NONE,
} MAIN_EVENT;

typedef struct {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
} MainWin;
/**
 *  Creates a new window for the main menu.
 *  @return
 *  An instant of the new main menu window.
 */
MainWin* mainWindowCreate();
/**
 *  Creates new widgets for the main menu window.
 *  @param-renderer - the renderer for the window.
 *  @return
 *  All widgets for the main menu window
 */
Widget** createMainWindowWidgets(SDL_Renderer* renderer);
/**
 *  Draws the current main menu window.
 *  @param-src - the main menu window that needs to been draw.
 */
void mainWindowDraw(MainWin* src);
/**
 *  Destroys the main menu window, uses the destroy widgets function
 *  also(that destroys all the widgets of the main menu window).
 *  @param-src - the main menu window that needs to been destroyed.
 */
void mainWindowDestroy(MainWin* src);
void mainWindowWidgetsDestroy(Widget** widgets);
/**
 * 	Handles all possible events, depending the option that the user has choose.
 *  @param-src - the main menu window, event - the event that the user activated.
 *  @return
 *  a MAIN_EVENT - depends on the event that has been sent by the user
 */
MAIN_EVENT mainWindowHandleEvent(MainWin* src, SDL_Event* event);
/**
 * 	Hides and Shows the main menu window.
 *  @param-src - the main menu window.
 */
void mainWindowHide(MainWin* src);
void mainWindowShow(MainWin* src);

#endif
