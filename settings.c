#include "settings.h"
Settings createDefaultSettings(){
	Settings settings;
	setSettings(&settings,DIFFICULTY_DEFAULT,ONE_PLAYER_MODE,USER_COLOR_DEFAULT);
	return settings;
}
int setSettings(Settings* settings,int level,int mode,int color){
	settings->game_level=setGameLevel(settings,level);
	settings->game_mode=setGameMode(settings,mode);
	settings->user_color=setGameColor(settings,color);
	return 1;
}
int setGameLevel(Settings* settings,int level){
	settings->game_level=level;
	return settings->game_level;
}
int setGameMode(Settings* settings,int mode){
	settings->game_mode=mode;
	return settings->game_mode;
}
int setGameColor(Settings* settings,int color){
	settings->user_color=color;
	return settings->user_color;
}
