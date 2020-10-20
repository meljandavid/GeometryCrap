#include"Editor.h"
#include"Game.h"
#include"MainMenu.h"
#include"Base.h"

int main() {
	Base game;

	game.pushState(new MainMenu(&game));
	game.GameLoop();

	return 0;
}