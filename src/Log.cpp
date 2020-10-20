#include "Log.h"

Log::Log() {
	font.loadFromFile("Assets/Roboto-Regular.ttf");
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(sf::Color::Black);
}

Log::~Log() {
	texts.clear();
}

void Log::render(sf::RenderWindow* window) {
	int it = 0;
	for (const std::pair<std::string, std::string>& ss : texts) {
		text.setPosition(sf::Vector2f(0.f, float(it)*(fontSize + 2.f)));
		text.setString(ss.second);
		window->draw(text);
		it++;
	}
}

void Log::set(const std::string& key, const std::string& text) {
	texts[key] = key+" "+text;
}