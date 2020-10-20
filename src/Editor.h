/*
Help: move with the arrows, zoom with the LSHIFT and the LCTRL
*/

#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/System/Clock.hpp>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<iomanip>

#include"Runnable.h"
#include"Objects.h"

struct eTile {
	bool highlight = false;
	int x = 0;
};

struct Button {
	sf::FloatRect hitbox;
	sf::Text text;
};

class Editor : public Runnable {
	sf::View edit;
	sf::View UI;
	const float split = .1f;
	double zoom = 1.f;
	std::stringstream sstream;

	std::ofstream out;
	std::vector<std::vector<eTile>> lvl;
	sf::Vector2f offset;
	sf::FloatRect mapBox, textureSelectorBox;
	int selectedTexture = 0;
	std::vector<Button> buttons;

	//PORTALs update START
	std::vector<Portal> portals;
	int ids = 0;
	//PORTALs update END

	bool rip = false;
	float dt;

	sf::RectangleShape shape;
	const float tileSize = 32.f;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text logText;

	float scrollSpeed;
	sf::Vector2i mouse;
	sf::Vector2f worldPos;

	void initVars();
	void loadMap();
	void updateView();
	void handleEvents();
	void modifyMap(int horizontal, int vertical);

public:
	int WIDTH = 960, HEIGHT = 640;

	Editor(Base* game);
	virtual ~Editor();

	virtual void render();
	virtual void update(const float _dt);
};