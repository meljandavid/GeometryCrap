#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System/Clock.hpp>
#include<vector>
#include<fstream>

#include"Verticemap.h"
#include"Player.h"
#include"Runnable.h"
#include"Objects.h"

class Game : public Runnable {
	sf::Image icon;
	sf::View view, ui;
	Player player;
	float tileSize;

	std::vector<std::vector<int>> level;
	std::vector<Turret> turrets;
	std::vector<Portal> portals;
	Verticemap verticemap;
	std::string file;

	sf::VertexArray bkg;
	sf::Color ctop, cbottom;

	bool rip = false;
	float respawn = 0.f;

	void initVars();
	void handleEvents();

public:
	Game(Base* game, const std::string &s="Assets/mymap.txt");
	virtual ~Game();

	virtual void update(const float dt);
	virtual void render();
};