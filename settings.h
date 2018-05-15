#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "piece.h"

typedef struct settings_t{
	int game_level;
	int game_mode;
	int user_color;
}Settings;
/**
 *  Creates new settings, as default.
 *  @return
 *  Default settings as asked in the instructions.
 */
Settings createDefaultSettings();
/**
 *  Sets a param of the settings, as rest of
 *  the param(thanks to rest of the setSettings functions).
 *  @param settings-the current instance of the settings,
 *  level,move,color - the settings that needs to update.
 *  @return
 *  1 on success, 0 else.
 */
int setSettings(Settings* settings,int level,int mode,int color);
int setGameLevel(Settings* settings,int level);
int setGameMode(Settings* settings,int mode);
int setGameColor(Settings* settings,int color);
#endif /* SETTINGS_H_ */
