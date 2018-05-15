#ifndef MAINAUX_H_
#define MAINAUX_H_
#include "parser.h"
/*
 * Starts a new game by waiting for settings from the user at console mode.
 * and passes it to the user input function.
 */
void newgame();
/*
 * Starts a new game with the settings that where inputed at new game.
 * At this function, the user and computer(or user vs user) performs the game, each by its turn.
 * @param - game-current game with settings from newgame function, loaded-if user loaded a game.
 */
void startGame(Game* game,bool loaded);
/*
 * Does a move if the computer starts the game.
 * A seperated function, because the first move(if computer) isn't in the history.
 * @param - game-current game.
 * @return game - after the computer move
 */
Game* firstMoveComputer(Game* game);
/*
 * Calculates and checks if the game is over - check-mate or tie situation.
 * @param- game-current game to check on.
 * @return 1 if game is over, 0 else.
 */
int gameOver(Game* game);
/*
 * Does a move if the computer starts the game.
 * @param - settings-the game settings, command - the command from the user, arg - the level.
 * @return true if the mode/difficulty is legal, false else.
 */
bool checkGameMode(Settings* settings,Command command);
bool checkDifficulty(Settings* settings,int arg);
/*
 * userTurn - manages the user turn. asks for the user his next move
 * and passes it to the user input function.
 * @return 1 - success
 */
int userTurn(Game* game);
/*
 * userHandler - manages the computer turn. calculates the best move to do
 * using the minimax algorithm and makes the move
 * @param game - the current game board
 * @return 1 on success,
 */
int computerTurn(Game* game);
/*
 * quit - quits the game. destroys the game and exits
 * @param game - the current game
 */
void quit(Game* game);
/*
 * restart - restarts the game. cleans the board and calls the function newgame
 * @param game - the current game
 */
void restart(Game* game);
/*
 * invalid - sends a ERROR
 */
void invalid();
/*
 * Preforms the move that has been sent.
 * @param-move-the move that has peen selected, game-current game.
 * @return 1 on success,0 else
 */
int move(Move move,Game* game);
/*
 * Saves the current game and the path of filename.
 * @param-filename-the path that the game will be saved, game-current game.
 * @return 1 on success,0 else
 */
int save(Game* game,char* filename);
/*
 * Loads the game from the path of filename.
 * @param-filename-the path of the game that will be loaded.
 * @return game-the game that has been loaded
 */
Game* load(char* fileName);
void loadForBlackPlayer(Game* game,char ch,int j,int i);
void loadForWhitePlayer(Game* game,char ch,int j,int i);
/*
 * After loading a game, this function takes care
 * to add an empty piece to the array of pieces for each user.
 * @param game - the current game
 */
void fillEmptyPieces(Game* game);
/*
 * After loading a game, this function takes care
 * to set the current player to make the next move,
 * according to the game that has just been loaded.
 * @param game - the current game, currentturn: 1 if white, 0 if black.
 */
void checkPlayerForLoad(Game* game,int currentturn);
/*
 * undo - undos the last two moves, the first one of the
 * computer's and the second the last move of the user
 * @param game - the current game
 * @return 1 on success, -1 on failure and 0 if the is no history
 */
int undo(Game* game);
/*
 * Waits for user input to choose the settings to start the game.
 * @param settings - the current game settings.
 * @return command-a command that holds the arguments
 * of the user inputs at the settings stage.
 */
Command gameSetting(Settings* settings);
/*
 * userInput - manages the user commands and acts accordingly.
 * @param game - the current game board
 * @return the Command according to the user's input
 */
Command userInput(Game* game);
/*
 * Check if the get moves option is valid for gameboard[y][x].
 * If so - prints the moves, else - a print with the relevant error.
 * @param game - the current game, x,y-place on gameboard to check the get moves option.
 */
void checkGetMoves(Game* game,int x, int y);
#endif /* MAINAUX_H_ */
