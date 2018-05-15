#ifndef PIECE_H_
#define PIECE_H_
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
//Definitions
#define MAX_LINE_LENGTH 1024
#define EXPERT_LEVEL 5
#define EMPTY_PIECE '_'
#define NUM_PIECES 16
#define GAME_TIE_SYMBOL '-'
#define GAME_SPAN 4
#define GAME_N_ROWS 8
#define GAME_N_COLUMNS 8
#define PLAYER_1_SYMBOL 'H'
#define PLAYER_2_SYMBOL 'C'
#define WHITE 1
#define BLACK 0
#define TWO_PLAYERS_MODE 2
#define ONE_PLAYER_MODE 1
#define USER_COLOR_DEFAULT 1
#define DIFFICULTY_DEFAULT 2
#define GAME_OVER 1
#define GAMECONTINUE 0

typedef struct piece_t{
	int x,y;
	char player;
	char type;
	int color;
	bool alive;
}Piece;
/**
 * Creates a new piece with a specified params.y size is a
 *
 * @param -x,y-place on gameboard, player - the symbol
 * of the player's piece, type - type of piece,
 * color - the color of the new piece.
 * @return
 * NULL if fail, else - the new piece.
 */
Piece* createPiece(int x,int y,char player,char type,int color);
/**
 *	Creates a copy of a given piece.
 *	The new copy has the same params as the original piece.
 *
 *	@param piece - the source piece which will be copied
 *	@return
 *	NULL if either piece is NULL or a memory allocation failure occurred.
 *	Otherwise, a new copy of the source piece is returned.
 */
Piece* pieceCopy(Piece* piece);
/**
 * Frees all memory allocation associated with a given piece. If piece==NULL
 * the function does nothing.
 *
 * @param piece - the source piece.
 */
void destroyPiece(Piece* piece);
/**
 *  Sets a piece as the param(thanks to rest of the setPiece functions).
 *  @param piece-the current piece that needs
 * to be set, x,y - place on gameboard of the piece.
 *  @return
 *  1 on success, 0 else.
 */
int setPiece(Piece* piece,int x,int y,char player,char type,int color,bool alive,bool moved);
int setPieceX(Piece* piece,int x);
int setPieceY(Piece* piece,int y);
int setPiecePlayer(Piece* piece,char player);
int setPieceType(Piece* piece,char type);
int setPieceColor(Piece* piece,int color);
int setEmptyPiece(Piece* piece,int x,int y);
int setPieceAlive(Piece* piece,bool alive);
/**
 *  Returns the score of the piece, as known for each piece.
 *  @param piece-the current piece.
 *  @return
 *  The score of the piece, according to the score that we got from the instructions.
 */
int getPieceScore(Piece* piece);
int getPieceScoreExpert(Piece* piece);
/**
 *  Returns the score of the piece, as known for each piece.
 *  @param piece-the current piece.
 *  @return
 *  true if the piece is still part of the game, false else.
 */
bool isPieceAlive(Piece* piece);
/**
 *  Moves a piece to gameboard[j][i].
 *  @param piece-the current piece, i,j- where to move the piece
 */
void movePiece(Piece* piece,int i,int j);
/**
 *  If the pieces color is black, the char will be upper-case, else - white.
 *  The char will be as told-first letter(knight-'n',pawn-'m').
 *  @param piece-the current piece.
 *	@return
 *	a char that represents the pieces type and color
 */
char getPiecePrint(Piece* piece);
/**
 *  Returns a string of the pieces with its color.
 *  @param piece-the current piece.
 *	@return
 *	a string of the pieces(full type and not char) with its color.
 */
char* getPieceName(Piece* piece);

#endif /* PIECE_H_ */
