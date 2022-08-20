#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/System/Clock.hpp>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<iomanip>

#include"State.h"
#include"Objects.h"
#include"Button.h"
#include"TextField.h"

struct eTile {
	bool highlight = false;
	int x = 0;
};

class Editor : public State {
	sf::View edit;
	sf::View UI;
	const float split = .1f;
	double zoom = 1.f;
	std::stringstream sstream;

	std::string filename;
	
	std::vector<std::vector<eTile>> lvl;
	sf::FloatRect mapBox, textureSelectorBox;
	int selected = 100;
	std::vector<Button> buttons;
	TextField tf_save;
	Button b_save;

	std::vector<Portal> portals;
	int ids = 0;
	std::string musicname, mapcolor;

	float dt;

	sf::RectangleShape shape;
	const float tileSize = 32.f;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Font font;
	sf::Text logText;

	float scrollSpeed, zoomfactor;
	bool rmhold = false;
	sf::Vector2i mouse, holdmouse;
	sf::Vector2f worldPos;

	void initVars();
	void loadMap();
	void updateView();
	void handleEvents();

public:
	int WIDTH = 960, HEIGHT = 640;

	Editor(Config* config, const std::string& editfile = "mymap");
	virtual ~Editor();

	virtual void render();
	virtual void update(const float _dt);
	virtual void resize(int width, int heigth);
};