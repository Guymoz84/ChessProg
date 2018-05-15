#ifndef AI_H_
#define AI_H_
#include "game.h"
/**
 *	Calculates the scoring of the game-board, according to the pieces of each user at the game.
 *	@param game - the current game, first - symbol of the player that called the function.
 *	@return
 *	the score, according to the score function from the instructions
 */
int scoring(Game* game,char first);
/**
 *	Calculates the scoring of the game-board, according to the pieces of each user at the game.
 *	@param game - the current game, first - symbol of the player that called the function.
 *	@return
 *	the score, according to the score function we thought of.
 */
int expertScoring(Game* game,char first);
/**
 *	Calculates the scoring of the game-board, according to sum of each user at the game.
 *	@param user1sum, user2sum - the user's scores, first - symbol of the player that called the function.
 *	@return
 *	the score, according to the first caller.
 */
int materialSum(int user1sum,int user2sum,char first);
/**
 *	Calculates and finds the best move to do, according to the bestMoveRec function.
 *	@param game - the current game
 *	@return
 *	the move with the max score for the first player who called this function.
 */
Move chooseBestMove(Game* game);
/**
 *	Calculates the max score, according to the ai algorithm that has been showed and the score function.
 *	@param game - the current game, alpha and beta - by the improved minimax algorithm,
 *	max_player - which player is been calculated to find the max score,
 *	first-symbol of the player that called the function,layer that called the function,
 *	depth - the current depth, according to the recursion.
 *	@return
 *	the max score for the first player who called the chooseBestMove function.
 */
int bestMoveRec(Game* game,int alpha,int beta,int max_player,char first,int depth);
/**
 * 	Standard max function
 *	@param x - int, y - int
 *	@return
 *	the max between x and y.
 */
int max(int x,int y);
/**
 * 	Standard min function
 *	@param x - int, y - int
 *	@return
 *	the min between x and y.
 */
int min(int x, int y);
#endif /* AI_H_ */
