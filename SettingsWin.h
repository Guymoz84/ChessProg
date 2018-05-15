#ifndef SettingsWin_H_
#define SettingsWin_H_
#include "settings.h"
#include "SimpleButton.h"

typedef enum {
	SETTINGS_EVENT_ONE_PLAYER,
	SETTINGS_EVENT_TWO_PLAYERS,
	SETTINGS_EVENT_NEXT,
	SETTINGS_EVENT_START,
	SETTINGS_EVENT_BACK,
	SETTINGS_NOOB,
	SETTINGS_EASY,
	SETTINGS_MODERATE,
	SETTINGS_HARD,
	SETTINGS_WHITE_COLOR,
	SETTINGS_BLACK_COLOR,
	SETTINGS_EVENT_QUIT,
	SETTINGS_EVENT_INVALID_ARGUMENT,
	SETTINGS_EVENT_NONE,
	SETTINGS_EVENT_UPDATE,
} SETTINGS_EVENT;

typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	Widget** widgets;
	int numOfWidgets;
	bool first;
	Settings settings;
	int page;
}SettingsWin;
/**
 *  Creates a new window for the settings option.
 *  @return
 *  An instant of the new settings window.
 */
SettingsWin* settingsWindowCreate();
/**
 *  Creates new widgets for the settings window.
 *  @param-renderer - the renderer for the window.
 *  @return
 *  All widgets for the settings window
 */
Widget** createSettingsWindowWidgets(SDL_Renderer* renderer);
/**
 *  Draws the current settings window.
 *  @param-src - the settings window that needs to been draw.
 */
void settingsWindowDraw(SettingsWin*);
/**
 *  Destroys the settings window, uses the destroy widgets function
 *  also(that destroys all the widgets of the settings window).
 *  @param-src - the settings window that needs to been destroyed.
 */
void settingsWindowDestroy(SettingsWin*);
void settingsWindowWidgetsDestroy(Widget** widgets);
/**
 * 	Handles all possible events, depending the choice of the users.
 *  @param-src - the settings window, event - the event that the user activated.
 *  @return
 *  a SETTINGS_EVENT - depends on the settings that the user chooses.
 */
SETTINGS_EVENT settingsWindowHandleEvent(SettingsWin* src, SDL_Event* event);
/**
 * 	Hides and Shows the settings window.
 *  @param-src - the settings window.
 */
void settingsWindowHide(SettingsWin* src);
void settingsWindowShow(SettingsWin* src);
#endif
