#include "Button.h"

Button::Button(
	const sf::FloatRect& box,
	const std::string& str)
{
	shape.setSize(sf::Vector2f(box.width, box.height));
	shape.setPosition(sf::Vector2f(box.left, box.top));

	text.setString(str);

	setDefaults();
}

void Button::setOnPress(std::function<void()> f) {
	onPress = f;
}

void Button::update(sf::Vector2f mouse, bool pressed) {
	if (shape.getGlobalBounds().contains(mouse)) {
		if (pressed) {
			shape.setFillColor(conpress);
			if (!prev) {
				onPress();
				prev = true;
			}
		}
		else {
			onHover();
			prev = false;
		}
	}
	else {
		setDefaults();
	}

	text.setPosition(sf::Vector2f(
		float(shape.getPosition().x + 0.5f * (shape.getLocalBounds().width - text.getLocalBounds().width)),
		float(shape.getPosition().y + 0.4f * (shape.getLocalBounds().height - text.getLocalBounds().height))
	));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(shape);
	target.draw(text);
}

void Button::onHover() {
	shape.setFillColor(cbkghover);
	shape.setOutlineThickness(2);

	text.setFillColor(ctexthover);
}

void Button::setDefaults() {
	shape.setOutlineThickness(0);
	shape.setFillColor(cbkg);
	shape.setOutlineColor(cborder);

	text.setFillColor(ctext);
}
