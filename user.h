#ifndef USER_H_
#define USER_H_
#include "piece.h"
typedef struct user_t{
	Piece* king;
	Piece* pieces[NUM_PIECES];
	int numpieces;
	int color;
	char symbol;
}User;
/**
 * Creates a new user with a specified color and symbol.
 *
 * @param -symbol&color - the symbol and color for the new user.
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new user instant is returned.
 */
User* userCreate(char symbol,int color);
/**
 *	Initalize a user by all pieces at the start of chess.
 *
 *	@param user - the source user which will be copied.
 *	settings - according to the user choice
 *	@return
 *	NULL if either game is NULL or a memory allocation failure occurred.
 *	Otherwise, a user with all its pieces.
 */
User* userInitalize(User* user);
/**
 *	Creates a copy of a given user.
 *	The new copy has the same params as the original user.
 *
 *	@param user - the source user which will be copied
 *	@return
 *	NULL if either user is NULL or a memory allocation failure occurred.
 *	Otherwise, a new copy of the source user is returned.
 */
User* userCopy(User* user);
/**
 * Frees all memory allocation associated with a given user. If user==NULL
 * the function does nothing.
 *
 * @param piece - the source piece.
 */
void userDestroy(User* user);
/**
 * Sets color and symbol to a given user.
 *
 * @param user - the source user, color(or symbol)-param to set to the user.
 * @return
 * 1 on success, 0 else.
 */
int setUserColor(User* user,int color);
int setUserSymbol(User* user,char symbol);
/**
 * Adds a new piece to the user.
 * @param user - the source user, x,y-coordinates of the new piece, type-type of piece.
 * @return
 * Instance of the piece that we just added.
 */
Piece* addUserPiece(User* user,int x,int y,char type);

#endif /* USER_H_ */
