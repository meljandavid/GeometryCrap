#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/System/Clock.hpp>
#include<vector>
#include<fstream>

#include"Verticemap.h"
#include"Player.h"
#include"State.h"
#include"Objects.h"

class Game : public State {
	sf::Image icon;
	sf::View view, ui;
	Player player;
	float tileSize;

	std::vector<std::vector<int>> level;
	std::vector<Turret> turrets;
	std::vector<Portal> portals;
	Verticemap verticemap;
	std::string file;
	std::string musicname, bkgcolor;

	sf::VertexArray bkg;
	sf::Color ctop, cbottom;

	float respawn = 0.f;

	void initVars();
	void handleEvents();

public:
	Game(Config* config, const std::string &s="mymap");
	virtual ~Game();

	virtual void update(const float dt);
	virtual void render();
	virtual void resize(int width, int height);
};