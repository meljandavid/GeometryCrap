#pragma once
#include"Button.h"
#include<optional>

class TextField : public Button {
public:
	TextField(const sf::FloatRect& box = sf::FloatRect(10.f, 10.f, 200.f, 100.f));
	void setTitle(const std::string& s);

	void update(sf::Vector2f mouse, bool pressed);
	void reset();
	void setCharCheckerFunc(std::function<bool(char c)> f);
	void setValCheckerFunc(std::function<bool(const std::string& s)> f);
	std::optional<std::string> getVal();
	void textInput(char c);
	void setInitialText(const std::string& init);

	sf::Color cbkgonpress = sf::Color(160,160,160),
		cbkg = sf::Color(224,224,224, 240); //darkgrey

private:
	using Button::setOnPress;

	std::string inText, def;
	bool hasFocus = false;
	std::function<bool(char c)> checkchar;
	std::function<bool(const std::string & c)> checkval;
	
	void onFocus();
	void onHover();
	void onDefault();
};

