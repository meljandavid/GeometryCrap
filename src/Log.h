/*
Simple log.
*/

#pragma once

#include<SFML/Graphics.hpp>
#include<unordered_map>
#include<iostream>

class Log {
	std::unordered_map<std::string, std::string> texts;
	sf::Font font;
	sf::Text text;
	float fontSize = 30.f;

public:
	Log();
	~Log();
	void render(sf::RenderWindow* window);
	void set(const std::string& key, const std::string& text);
};
