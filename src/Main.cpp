#include"MainMenu.h"
#include"StateManager.h"

int main() {
	StateManager sm;

	sm.pushState(
		std::make_unique<MainMenu>( &(sm.config ))
	);
	sm.GameLoop();

	return 0;
}