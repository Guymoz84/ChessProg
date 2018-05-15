#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_

#include <SDL_events.h>
#include <SDL_messagebox.h>
#include "SettingsWin.h"
#include "LoadWin.h"
#include "SaveWin.h"
#include "MainWin.h"
#define NUM_OF_SLOTS 5
typedef enum{
	MAIN_WINDOW_ACTIVE,
	GAME_WINDOW_ACTIVE,
	SETTINGS_WINDOW_ACTIVE,
	LOAD_WINDOW_ACTIVE,
	SAVE_WINDOW_ACTIVE,
}ACTIVE_WINDOW;

typedef enum{
	MANAGER_QUIT,
	MANAGER_NONE,
	MANAGER_UPDATE,
}MANAGER_EVENET;

typedef struct {
	SettingsWin* settingsWin;
	LoadWin* loadWin;
	SaveWin* saveWin;
	GameWin* gameWin;
	MainWin* mainWin;
	ACTIVE_WINDOW activeWin;
} GuiManager;
/**
 *  Creates a new window for the menu.
 *  @return
 *  An instant of the new menu window.
 */
GuiManager* managerCreate();
/**
 *  Destroys the main menu window, uses the destroy widgets function
 *  also(that destroys all the widgets of the menu window).
 *  @param-src - the main window that needs to been destroyed.
 */
void managerDestroy(GuiManager* src);
/**
 *  Selects witch window to draw, according to the active windoww.
 *  @param-src - all options of possible windows that can be active or not.
 */
void managerDraw(GuiManager* src);
/**
 * 	Handles all possible events, depending the game situation and the active window.
 * 	Sends to the handle event of the active window.
 *  @param-src - all options of possible windows that can be active or not,event- type of event from the user
 *  @return
 *  a MANAGER_EVENT - depends on the event that has been sent by the user
 */
MANAGER_EVENET handleManagerDueToMainEvent(GuiManager* src,MAIN_EVENT event);
MANAGER_EVENET handleManagerDueToSettingsEvent(GuiManager* src,SETTINGS_EVENT event);
MANAGER_EVENET handleManagerDueToLoadEvent(GuiManager* src,LOAD_EVENT event);
MANAGER_EVENET handleManagerDueToGameEvent(GuiManager* src,GAME_EVENT event);
/**
 *  Takes care of an event that saves the current game.
 *  @param-src - the current instance from the handke Manager Game event
 */
void saveGui(GuiManager* src);
MANAGER_EVENET managerHandleEvent(GuiManager* src, SDL_Event* event);

#endif
