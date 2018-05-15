CC = gcc
OBJS = main.o Widget.o user.o SimpleButton.o SettingsWin.o settings.o SaveWin.o PieceButton.o piece.o parser.o move.o MainWin.o maingui.o mainconsole.o mainAux.o LoadWin.o GUIManager.o GameWin.o game.o error.o Common.o ArrayList.o ai.o
 
EXEC = chessprog
CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $(EXEC)
main.o: main.c mainconsole.h maingui.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ai.o: ai.c ai.h game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ArrayList.o: ArrayList.c ArrayList.h move.h error.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Common.o: Common.c Common.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
error.o: error.c error.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
game.o: game.c game.h ArrayList.h settings.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameWin.o: GameWin.c GameWin.h SimpleButton.h PieceButton.h mainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUIManager.o: GUIManager.c GUIManager.h SettingsWin.h LoadWin.h SaveWin.h MainWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
LoadWin.o: LoadWin.c LoadWin.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
mainAux.o: mainAux.c mainAux.h parser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
mainconsole.o: mainconsole.c mainconsole.h mainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
maingui.o: maingui.c maingui.h GUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
MainWin.o: MainWin.c MainWin.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
move.o: move.c move.h user.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
parser.o: parser.c parser.h ai.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
piece.o: piece.c piece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
PieceButton.o: PieceButton.c PieceButton.h Widget.h Common.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SaveWin.o: SaveWin.c SaveWin.h GameWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
settings.o: settings.c settings.h piece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SettingsWin.o: SettingsWin.c SettingsWin.h settings.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SimpleButton.o: SimpleButton.c SimpleButton.h Common.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
user.o: user.c user.h piece.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
