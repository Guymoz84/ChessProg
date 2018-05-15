#ifndef GAMEWIN_H_
#define GAMEWIN_H_
#include <SDL.h>
#include <SDL_video.h>
#include "SimpleButton.h"
#include "PieceButton.h"
#include "mainAux.h"

typedef enum {
	GAME_EVENT_TIE,
	GAME_EVENT_UNDO,
	GAME_EVENT_RESET,
	GAME_EVENT_SAVE,
	GAME_EVENT_LOAD,
	GAME_EVENT_MAIN_MENU,
	GAME_EVENT_EXIT,
	GAME_EVENT_INVALID_ARGUMENT,
	GAME_EVENT_NONE,
	GAME_EVENT_UPDATE,
} GAME_EVENT;

typedef struct{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* bgTexture;
	SDL_Texture* redTexture;
	SDL_Texture* blueTexture;
	SDL_Texture* greenTexture;
	Widget** widgets;
	int numOfWidgets;
	bool first;
	bool saved;
	bool back_to_mainmenu;
	bool pawn_promotion;
	ArrayList* moves;
	Game* game;
}GameWin;
/**
 *  Creates a new window for the game.
 *  @return
 *  An instant of the new game window.
 */
GameWin* gameWindowCreate();
/**
 *  Creates Textures for the game board for the get moves option.
 *  @param-res - the Game window, loadingSurgace - to load an image for the new texture
 *  @return
 *  The updated game window with the new texture
 */

GameWin* createGreenTexture(GameWin* res,SDL_Surface* loadingSurface);
GameWin* createRedTexture(GameWin* res,SDL_Surface* loadingSurface);
GameWin* createBlueTexture(GameWin* res,SDL_Surface* loadingSurface);
/**
 *  Creates new widgets for the game window.
 *  @param-renderer - the renderer for the window, game - current game
 *  @return
 *  All widgets for the game window
 */
Widget** createGameWindowWidgets(SDL_Renderer* renderer,Game* game);
/**
 *  Draws the current game window.
 *  @param-src - the game window that needs to been draw.
 */
void gameWindowDraw(GameWin* src,bool update);
void drawUpdateGameWindow(GameWin* src);
/**
 *  Destroys the game window, uses the destroy widgets function
 *  also(that destroys all the widgets of the game window).
 *  @param-src - the game window that needs to been destroyed.
 */
void gameWindowDestroy(GameWin* src);
void gameWindowWidgetsDestroy(Widget** widgets,Game* game);
/**
 * 	Handles all possible events, depending the game situation, current player, etc.
 *  @param-src - the game window, event - the event that the user activated.
 *  @return
 *  a GAME_EVENT - depends on the event that has been sent by the user
 */
GAME_EVENT gameWindowHandleEvent(GameWin* src, SDL_Event* event);
GAME_EVENT buttonUp(GameWin* src,SDL_Event* event);
GAME_EVENT buttonDown(GameWin* src,SDL_Event* event);
/**
 * 	Preforms a computer move at Gui mode.
 *  @param-src - the game window.
 *  @return
 *  a GAME_EVENT that is chosen by the game situation.
 */
GAME_EVENT computerMoveGui(GameWin* src);
/**
 * 	Check if there is a check,check-mate or tie situation and
 *  shows a pop-up box with the current situation.
 *  @param-src - the game window,gameover- a message if there is one of 3 situations.
 */
void checkMateOrTie(GameWin* src,GAME_MESSAGE gameover);
/**
 * 	Check if there is a check,check-mate or tie situation and
 *  shows a pop-up box with the current situation.
 *  @param-src - the game window,move- the last move that
 *  has been done to check if it's a pawn promotion.
 *  @return true if there is a pawn promotion, else-false.
 */
bool checkPromotion(GameWin* src, Move move);
/**
 * 	Hides and Shows the game window.
 *  @param-src - the game window.
 */
void gameWindowHide(GameWin* src);
void gameWindowShow(GameWin* src);

#endif
