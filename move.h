#ifndef MOVE_H_
#define MOVE_H_
#include "user.h"

typedef struct move_t{
	int x,y,i,j;
	Piece* eaten;
	bool eatmove;
	int eval;
}Move;
/**
 *  Sets a move as the param(thanks to rest of the setMove functions).
 *  @param move-the current move that needs
 * to be set, x,y,i,j - coordinations of the move.
 *  @return
 *  1 on success, 0 else.
 */
int setMove(Move* move,int x,int y,int i,int j);
int setMoveX(Move* move,int x);
int setMoveY(Move* move,int y);
int setMoveI(Move* move,int i);
int setMoveJ(Move* move,int j);
/**
 *  Checks if a move is a move that eats another piece.
 *  @param move - the current move that the needs to be checked.
 *  @return
 *  2 if a move is an eat-move, 0 else.
 */
int checkIfEatMove(Move move);

#endif /* MOVE_H_ */
