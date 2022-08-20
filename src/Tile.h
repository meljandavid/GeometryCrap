#pragma once
#include"SFML/System/Vector2.hpp"

struct Tile {
	bool isLethal, isCollidable;

	Tile(bool iL, bool iC) {
		isCollidable = iC;
		isLethal = iL;
	}
};

namespace Tiles {
	static Tile Wall(false, true);
	static Tile Trap(true, false);
	static Tile Air(false, false);
	static Tile Illusion(false, false);
	static Tile Bait(false, true);
	static Tile Portal(false, false);
	static Tile Turret(false, false);
}

static Tile tileArr[7] = {
	Tiles::Air, Tiles::Wall, Tiles::Illusion, Tiles::Bait, Tiles::Portal, Tiles::Turret, Tiles::Trap
};

