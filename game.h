#ifndef GAME_H_
#define GAME_H_
#include "ArrayList.h"
#include "settings.h"
#define HISTORY_SIZE 6
#define CHANGE_FOR_PRINT 65
/**
 * Game Summary:
 *
 * A container that represents a classic chess game, can play two players 8 by 8
 * board game (rows X columns). The container supports the following functions.
 *
 * GameCreate           - Creates a new game board
 * GameCopy             - Copies a game board
 * GameDestroy          - Frees all memory resources associated with a game
 * GameSetMove          - Sets a move on a game board
 * GameIsValidMove      - Checks if a move is valid
 * GameUndoPrevMove     - Undoes previous move made by the last player
 * GamePrintBoard       - Prints the current board
 * GameGetCurrentPlayer - Returns the current player
 *
 */
typedef enum game_message_t {
	GAME_INVALID_MOVE,
	GAME_INVALID_ARGUMENT,
	GAME_NO_HISTORY,
	GAME_SUCCESS,
	GAME_CHECK,
	GAME_TIE,
	GAME_CHECKMATE,
	GAME_CONTINUE
} GAME_MESSAGE;

typedef struct game_t {
	Settings settings;
	Piece* gameBoard[GAME_N_ROWS][GAME_N_COLUMNS];
	User* user1;
	User* user2;
	char currentPlayer;
	ArrayList* history;
}Game;
/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @param -historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
Game* gameCreate(int historySize);
/**
 *	Initalize a game by the settings.
 *
 *	@param game - the source game which will be copied.
 *	settings - according to the user choice
 *	@return
 *	NULL if either game is NULL or a memory allocation failure occurred.
 *	Otherwise, a game with the settings.
 *
 */
Game* gameInitalize(Game* game,Settings settings);
/**
 * Frees all memory allocation associated with a given game. If game==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void gameDestroy(Game* game);
/**
 * Adds the move to the arraylist and sends to the gameSetMoveAux function.
 *
 * @param src - The target game
 * @param move - The move that the user/computer has chosen to do
 * @return
 * GAME_INVALID_MOVE - if src is NULL or the move is invalid.
 * GAME_SUCCESS - if the given move has been done(by the gameSetMoveAux).
 */
GAME_MESSAGE gameSetMove(Game* game, Move move);
/**
 * Sets the next move in a given game by specifying the move.
 *
 * @param src - The target game
 * @param move - The move that the user/computer has chosen to do
 * @return
 * 1 - if the given move has been done.
 */
int gameSetMoveAux(Game* game,Move* move);
/**
 * Checks if a move can be done in the specified game, according to all the chess rules.
 *
 * @param game - The source game
 * @param move - The specified move
 * @return
 * true  - if the move can be done at the current game
 * false - otherwise.
 */
bool gameIsValidMove(Game* game,Move* move);
/**
 * Checks if a move can be done in the specified
 * game(every piece),but no care about check/mate.
 *
 * @param game - The source game
 * @param move - The specified move
 * @return
 * true  - if that move can be done at the current game
 * false - otherwise.
 */
bool gamePseudoLegalMove(Game* game, Move move);
/**
 * Sends to undoLastMove, that removes the last move
 * If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param game - The source game
 * @return
 * GAME_INVALID_ARGUMENT - if game == NULL
 * GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
GAME_MESSAGE gameUndoPrevMove(Game* game);
/**
 * Removes the last move from the history of the game.
 *
 * @param game - The source game
 */
void undoLastMove(Game* game);
/**
 * Removes the last move that was done.
 * @param game - The source game
 */
void undoLastMoveAux(Game* game,Move* move);
/**
 * Changes the turn of the players.
 * @param game - The source game
 */
void changeTurn(Game* game);
/**
 * Prints the current game, according to the instructions.
 * @param game - The source game
 */
GAME_MESSAGE printGameBoard(Game* game);
/**
 * Checks who is the user, that its turn to make the next move
 * @param game - The source game
 * @return
 * User - the user, that its turn to make the next move
 */
User* gameGetCurrentPlayer(Game* game);
/**
 * Checks the game board and tests if the is a check/mate according to the game board.
 * @param game - The source game
 * @return
 * GAME_CHECKMATE - if there is a checkmate for one of the players.
 * GAME_TIE - if there is a tie.
 * GAME_CHECK - if there is a check for one of the players.
 */
GAME_MESSAGE checkWinnerOrTie(Game* game);
/**
 * All validMove functions checks if a move can be done
 * by a specific type of piece in the game,but no care about check/mate.
 *
 * @param game - The source game
 * move - a move that needs to been verify if legal
 * @return
 * return true if the move can be done by the same piece
 * else, will return false
 */
bool validMoveRook(Game* game,Move move);
bool validMoveBishop(Game* game,Move move);
bool validMoveKnight(Game* game,Move move);
bool validMoveQueen(Game* game,Move move);
bool validMoveKing(Game* game,Move move);
bool validMovePawn(Game* game,Move move);
bool validMovePawnBlack(Game* game,Move move);
bool validMovePawnWhite(Game* game,Move move);
/**
 * All getMoves functions finds all possible moves for a current piece, by
 * all the pieces, according to the current player if a move can be done
 * and checked by insertMoves and setAndInsert if a move is valid
 * @param game - The source game
 * piece - whom would we like to find all possible moves
 * @return
 * An ArrayList of all the moves(empty if none)
 */
ArrayList* getMoves(Game* game,Piece* piece);
ArrayList* getMovesRook(Game* game,Piece* piece);
ArrayList* getMovesBishop(Game* game,Piece* piece);
ArrayList* getMovesKnight(Game* game,Piece* piece);
ArrayList* getMovesQueen(Game* game,Piece* piece);
ArrayList* getMovesKing(Game* game,Piece* piece);
ArrayList* getMovesPawn(Game* game,Piece* piece);
ArrayList* getMovesPawnWhite(Game* game,ArrayList* moves,Piece* piece);
ArrayList* getMovesPawnBlack(Game* game,ArrayList* moves,Piece* piece);
void insertMoves(Game* game,Move move,ArrayList* moves);
void setAndInsert(Game* game,ArrayList* moves,Piece* piece,int x,int y,int i,int j);
/**
 * Uses other functions to calculate if the is a check situation or not.
 * @param game - The source game.
 * @return
 * true if the is a check situation, else - returns false.
 */
bool gameCheck(Game* game);
/**
 * All threat functions finds(if there is) a piece that threatens
 *  a current piece that is given, thanks to i,j.
 * @param game - The source game
 * i,j - the threatened piece(gameboard[j][i])
 * @return
 * A Piece that is threatening the piece in gameboard[j][i].
 */
Piece* gameThreat(Game* game,int i,int j);
Piece* threatByPawn(Game* game,int i,int j,char type);
Piece* threatByWhitePawn(Game* game,int i,int j,char type);
Piece* threatByBlackPawn(Game* game,int i,int j,char type);
Piece* threatByBishop(Game* game,int i,int j,char type);
Piece* threatDiagToLeftUpBishop(Game* game,int i,int j,char type);
Piece* threatDiagToRightUpBishop(Game* game,int i,int j,char type);
Piece* threatDiagToLeftDownBishop(Game* game,int i,int j,char type);
Piece* threatDiagToRightDownBishop(Game* game,int i,int j,char type);
Piece* threatByKnight(Game* game,int i,int j,char type);
Piece* threatKnightHelper(Game* game,int i,int j,char type);
Piece* threatByRook(Game* game,int i,int j,char type);
Piece* threatUpRook(Game* game,int i,int j,char type);
Piece* threatDownRook(Game* game,int i,int j,char type);
Piece* threatLeftRook(Game* game,int i,int j,char type);
Piece* threatRightRook(Game* game,int i,int j,char type);
Piece* threatByQueen(Game* game,int i,int j,char type);
Piece* threatByKing(Game* game,int i,int j,char type);
Piece* threatBasic(Game* game,Move move);
/**
 * Both of findThreat functions finds(if there is) a piece that threatens
 *  a current piece that is given, thanks to i,j and moves all possible places on the board to find that threat.
 * @param game - The source game
 * i,j - the threatened piece(gameboard[j][i])
 * sum1,sum2 - sum1 is how many we need to add to j and
 * look at the borad(so is sum2 to i) - at gameboard[j+sum1][i+sum2]
 * @return
 * A Piece that is threatening the piece in gameboard[j][i].
 */
Piece* findThreat(Game* game,int i,int j,int sum1,int sum2,char type);
Piece* findThreatBasic(Game* game,int i,int j,int sum1,int sum2,char type);
/**
 * Uses other functions to calculate if the is a check-mate situation or not.
 * @param game - The source game.
 * @return
 * true if the is a check-mate situation, else - returns false.
 */
bool gameCheckmate(Game* game);
/**
 * Uses other functions to find if the current player can block the other at a check situation.
 * @param game - The source game.
 * king - the current's turn king
 * kingthreat - who is treating the king
 * @return
 * true if the is a piece to block,else - returns false.
 */
bool gameBlockThreat(Game* game,Piece* king,Piece* kingthreat);
bool gameBlockThreatBishop(Game* game,Piece* king,Piece* kingthreat);
bool gameBlockThreatRook(Game* game,Piece* king,Piece* kingthreat);
bool gameBlockThreatQueen(Game* game,Piece* king,Piece* kingthreat);
/**
 * Uses other functions to calculate if the is a tie situation or not.
 * @param game - The source game.
 * @return
 * true if the is a tie situation, else - returns false.
 */
bool gameTie(Game* game);
/**
 * Calculates if the param are valid indexes at the game board.
 * @param x,y -the indexes.
 * @return
 * true if the indexes are valid, else - returns false.
 */
bool indexIsValid(int x,int y);
/**
 * Prints all possible moves when using the get_moves option, sorted as asked.
 * @param game -the current game, arraylist- all possible moves, by indexes x,y.
 */
void printArrayListForGetMoves(ArrayList* arraylist,Game* game,int x,int y);
#endif
