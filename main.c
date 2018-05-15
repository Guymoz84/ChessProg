#include "mainconsole.h"
#include "maingui.h"
int main(int argc, char** argv){
	// start a console game
	if(argc == 2 && strcmp(argv[1], "-c") == 0) {
		return mainconsole();
	}
	// start a game with GUI
	else if(argc == 2 && strcmp(argv[1], "-g") == 0) {
		return maingui();
	}
	else {//default
		return mainconsole();//default!
	}
}
