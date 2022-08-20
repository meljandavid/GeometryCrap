/*
Simple log.
*/

#pragma once

#include<SFML/Graphics.hpp>
#include<map>
#include<iostream>
#include<fstream>

class Log {
	std::map<std::string, std::string> texts;
	sf::Font font;
	sf::Text text;
	float fontSize = 30.f;
	std::ofstream logf;

public:
	Log();
	~Log();

	void print(const std::string& msg);
	void render(sf::RenderWindow* window);
	void set(const std::string& key, const std::string& text);
};
