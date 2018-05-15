#ifndef SPCOMMON_H_
#define SPCOMMON_H_
#include <SDL.h>
#include <SDL_video.h>
/**
 * Copies the src to a new Rect
 * @param src - the source rect
 * @return
 * NULL if src is NULL
 * else, a new SDL_Rect with the same coordinates as the src
 */
SDL_Rect* copyRect(SDL_Rect* src);

#endif
