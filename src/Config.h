#pragma once

#include<fstream>
#include<SFML/Graphics/RenderWindow.hpp>

struct Config {
	sf::RenderWindow window;
	float volume = 1.f;
	
	Config() {
		window.create(sf::VideoMode(1280, 720), "GeometryCrap");
		window.setVerticalSyncEnabled(false);

		std::ifstream settings("Assets/settings.txt");
		if (settings) {
			std::string key; float val;
			while (settings >> key >> val) {
				if (key == "volume") volume = val;
			}
			settings.close();
		}
	}

	~Config() {
		window.close();

		std::ofstream settings("Assets/settings.txt");
		settings << "volume " << volume << std::endl;
		settings.close();
	}
};