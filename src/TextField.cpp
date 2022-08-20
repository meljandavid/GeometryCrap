#include "TextField.h"

#include<iostream>

TextField::TextField(const sf::FloatRect& box) {
	shape.setSize(sf::Vector2f(box.width, box.height));
	shape.setPosition(sf::Vector2f(box.left, box.top));

	text.setCharacterSize(40);

	checkchar = [](char c) { return (c >= 'a' && c <= 'z');  };
	checkval = [=](const std::string& s) { return inText.size() > 0; };

	onDefault();
}

void TextField::setTitle(const std::string& s) {
	text.setString(s);
	def = s;
}

void TextField::update(sf::Vector2f mouse, bool pressed) {
	if (inText.back() == '.')
		inText.pop_back();
	text.setString(inText);

	if (hasFocus) {
		onFocus();
	}
	else {
		onDefault();
	}

	if (shape.getGlobalBounds().contains(mouse)) {
		if(!hasFocus) onHover();
		if (pressed) {
			hasFocus = true;
		}
	}
	else {
		if (pressed) {
			hasFocus = false;
		}
	}

	text.setPosition(sf::Vector2f(
		float(shape.getPosition().x + 0.5f * (shape.getLocalBounds().width - text.getLocalBounds().width)),
		float(shape.getPosition().y + 0.3f * (shape.getLocalBounds().height - text.getLocalBounds().height))
	));
}

void TextField::setValCheckerFunc(std::function<bool(const std::string& s)> f) {
	checkval = f;
}

std::optional<std::string> TextField::getVal() {
	if (!checkval(inText)) return std::nullopt;
	return inText;
}

void TextField::textInput(char c) {
	if (hasFocus && checkchar(c))
		inText += c;
}

void TextField::setInitialText(const std::string& init) {
	inText = init;
}

void TextField::reset() {
	inText = "";
}

void TextField::setCharCheckerFunc(std::function<bool(char c)> f) {
	checkchar = f;
}

void TextField::onFocus() {
	shape.setFillColor(cbkgonpress);
	shape.setOutlineThickness(4);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
		if (inText.size() >= 1 && !prev) {
			inText.pop_back();
		}
		prev = true;
	}
	else {
		prev = false;
	}

	text.setString(inText + ".");
}

void TextField::onHover() {
	shape.setOutlineThickness(2);

	text.setFillColor(ctexthover);
}

void TextField::onDefault() {
	shape.setOutlineThickness(1);
	shape.setFillColor(cbkg);
	shape.setOutlineColor(cborder);

	text.setFillColor(ctext);
	if (inText.size() == 0) {
		text.setString(def);
	}
}
