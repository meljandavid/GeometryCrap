#pragma once
#include<SFML/Graphics.hpp>
#include<functional>

class Button : public sf::Drawable, public sf::Transformable {
public:
	Button(const sf::FloatRect &box = sf::FloatRect(10.f, 10.f, 200.f, 100.f),
		const std::string &str = "press me");

	void setOnPress(std::function<void()> f);
	void update(sf::Vector2f mouse, bool pressed);

	sf::Color cbkg = sf::Color(192, 192, 192, 240),
			  ctext = sf::Color::Black,
			  cborder = sf::Color::Black,
			  cbkghover = sf::Color(128, 128, 128), //darkgrey
			  ctexthover = sf::Color::White,
			  conpress = sf::Color::Yellow;

	sf::Text text;
	sf::RectangleShape shape;

protected:
	std::function<void()> onPress;
	bool prev = false;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onHover();
	void setDefaults();
};
