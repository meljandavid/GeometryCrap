#pragma once

#include"SFML/Graphics.hpp"
#include<vector>

struct Portal {
	std::pair<sf::Vector2u, sf::Vector2u> portals;
	int id=0;
	bool used = false;
};

class Turret {
	float elapsed = 0.f;
	const float reloadTime = 2.f;
	bool active;
	sf::Vector2f v_bullet = sf::Vector2f(-150.f, 0.f);
	sf::Vector2f start, offset=sf::Vector2f(0.f, 13.f);
	sf::CircleShape shape;

public:
	Turret(sf::Vector2f _pos) {
		active = true;
		start = _pos;
		shape.setPosition(_pos+offset);
		shape.setRadius(2.f);
		shape.setScale(sf::Vector2f(3.f, 0.5f));
		shape.setFillColor(sf::Color::Red);
	}

	void reset() {
		elapsed = 0.f;
		shape.setPosition(start+offset);
	}

	void update(float dt) {
		shape.move(v_bullet * dt);
		elapsed += dt;
		if (elapsed >= reloadTime)
			reset();
	}

	void render(sf::RenderWindow* window) {
		window->draw(shape);
	}

	sf::Vector2f getBulletPos() { return shape.getPosition(); }
	void setActive(bool b) { active = b; }
	sf::Vector2f getPos() { return shape.getPosition(); }
	bool isActive() { return active; }
};