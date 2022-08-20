#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio/Music.hpp>
#include<iostream>
#include<fstream>
#include<vector>

#include"Tile.h"
#include"Config.h"

class Player {
	float tileSize = 32.f;
	int HEIGHT;

	sf::Vector2f vel;
	float jumpSize;
	int jumps;
	sf::Vector2f gravity;
	bool isLive, canJump, hasWon;
	const std::vector<std::vector<int>>* lvl;
	
	Config* config = nullptr;
	sf::Music music;
	sf::Texture texture;
	sf::Font font;
	sf::Text text;
	int deaths = 0;

	void handleInput();
	void updateMovement(float dt);

public:
	sf::RectangleShape shape;
	bool jumpReady;

	Player();
	~Player();

	void initVars();
	void reset();
	void update(float dt);
	void render(sf::RenderWindow* window);
	void renderUI(sf::RenderWindow* window);
	void Jump();

	void setGravity(sf::Vector2f grav);
	void setLevel(std::vector<std::vector<int>>* v);
	void setMusic(const std::string name);
	void setConfig(Config* config);

	sf::Vector2f getGravity();
	bool isLiving();
	void setLive(bool b);
	bool getState();
};