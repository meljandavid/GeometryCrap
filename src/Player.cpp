#include "Player.h"

void Player::initVars() {
	jumpSize = 550.f;
	gravity = sf::Vector2f(0, 2000.f);

	texture.loadFromFile("Assets/Textures/playerTexture.jpg");
	int gap = 1;
	shape = sf::RectangleShape(sf::Vector2f(tileSize - gap, tileSize - gap));
	shape.setTexture(&texture);
	
	jumpSize = 550.f;

	hasWon = false;

	reset();

	if (!font.loadFromFile("Assets/Fonts/FoughtKnight.ttf"))
		std::cout << "failed to load fonts\n";
	text.setFont(font);
	text.setOutlineColor(sf::Color::Black);
}

void Player::reset() {
	music.setPlayingOffset(sf::seconds(0));
	music.setVolume(0.f);
	music.play();
	
	isLive = true;
	canJump = false;
	jumpReady = true;
	jumps = 0;
	shape.setPosition(sf::Vector2f(1.f, 1.f));
	vel.x = 0.f;
	text.setString(std::to_string(deaths));
}

void Player::handleInput() {
	if (jumps > 0 && canJump) {
		jumps--;
		vel.y = -jumpSize;
		canJump = false;
	}
}

void Player::updateMovement(float dt) {
	if (music.getVolume() < config->volume*100.f) {
		music.setVolume(std::min(music.getVolume() + dt * 50.f, config->volume * 100.f));
	}
	if (lvl->size() < 2) return;

	vel += gravity * dt;

	int Y1 = int(shape.getPosition().y + vel.y * dt) / int(tileSize);
	int Y2 = int(shape.getPosition().y + shape.getSize().y + vel.y * dt) / int(tileSize);
	int currX = int(shape.getPosition().x) / int(tileSize);
	int nextX = int(shape.getPosition().x + shape.getSize().x) / int(tileSize);

	if (!(currX >= 0 && currX + 1 < lvl->at(0).size() && Y1 >= 0 && Y2 < lvl->size()
		&& !tileArr[lvl->at(Y1).at(currX)].isCollidable && !tileArr[lvl->at(Y2).at(currX)].isCollidable
		&& !tileArr[lvl->at(Y1).at(nextX)].isCollidable && !tileArr[lvl->at(Y2).at(nextX)].isCollidable)) {
		if (currX >= 0 && nextX < lvl->at(0).size() && Y1 >= 0 && Y2 < lvl->size()) {
			if (tileArr[lvl->at(Y2).at(currX)].isCollidable || tileArr[lvl->at(Y2).at(currX + 1)].isCollidable) {
				jumps = 2;
				vel.x = jumpSize / 2;
			}
		}
		vel.y = 0;
	}

	if (Y2 >= lvl->size()) {
		setLive(false);
		return;
	}

	Y1 = int(shape.getPosition().y) / int(tileSize);
	Y2 = int(shape.getPosition().y + shape.getSize().y) / int(tileSize);
	currX = int(shape.getPosition().x + vel.x * dt) / int(tileSize);
	nextX = int(shape.getPosition().x + shape.getSize().x + vel.x * dt) / int(tileSize);

	if (currX+1 >= lvl->at(0).size()) {
		setLive(false);
		hasWon = true;
		return;
	}

	if (currX >= 0 && nextX < lvl->at(0).size() && Y1 >= 0 && Y2 < lvl->size()
		&& !tileArr[lvl->at(Y1).at(nextX)].isCollidable && !tileArr[lvl->at(Y2).at(currX)].isCollidable
		&& !tileArr[lvl->at(Y1).at(currX)].isCollidable && !tileArr[lvl->at(Y2).at(nextX)].isCollidable) {
		if (tileArr[lvl->at(Y2).at(currX)].isLethal || tileArr[lvl->at(Y2).at(nextX)].isLethal ||
			tileArr[lvl->at(Y1).at(currX)].isLethal || tileArr[lvl->at(Y1).at(nextX)].isLethal) {
			setLive(false);
			return;
		}
		shape.move(vel * dt);
	}
	else {
		setLive(false);
	}
}

Player::Player() {
	initVars();
	reset();
}

Player::~Player() { }

void Player::render(sf::RenderWindow* window) {
	sf::View newView = window->getView();
	HEIGHT = newView.getSize().y;
	window->draw(shape);

	float h = (shape.getPosition().y - newView.getCenter().y) / newView.getSize().y;
	float newy = newView.getCenter().y;
	if (h > 0.1) {
		newy = shape.getPosition().y - 0.1 * newView.getSize().y;
	}
	else if (h < -0.3) {
		newy = shape.getPosition().y + 0.3 * newView.getSize().y;
	}

	newView.setCenter(sf::Vector2f(shape.getPosition().x + newView.getSize().x/4.f, newy));
	window->setView(newView);
}

void Player::renderUI(sf::RenderWindow* window) {
	sf::View newView = window->getView();

	if (hasWon) {
		text.setOutlineThickness(1.f);
		text.setString("u won!");
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(newView.getSize().y / 4);
		text.setPosition(sf::Vector2f(newView.getCenter().x - text.getGlobalBounds().width / 2,
			newView.getCenter().y - text.getGlobalBounds().height));
		window->draw(text);

		text.setOutlineThickness(0);
		text.setString("deaths: " + std::to_string(deaths - 1));
		text.setFillColor(sf::Color::Red);
		text.setCharacterSize(newView.getSize().y / 8);
		text.setPosition(sf::Vector2f(newView.getCenter().x - text.getGlobalBounds().width / 2,
			newView.getCenter().y));
		window->draw(text);
	}
	else {
		text.setCharacterSize(newView.getSize().y / 8);
		text.setPosition(sf::Vector2f(newView.getCenter().x - newView.getSize().x / 2.2,
			newView.getCenter().y - newView.getSize().y / 2 + 1.f));
		window->draw(text);
	}
}

void Player::Jump() {
	if (jumpReady) {
		canJump = true;
		jumpReady = false;
	}
}

void Player::setGravity(sf::Vector2f grav) {
	gravity = grav;
}

void Player::setLevel(std::vector<std::vector<int>> *v) {
	lvl = v;
}

void Player::setMusic(const std::string name) {
	if (!music.openFromFile("Assets/Music/" + name)) {
		std::cout << "rip music :( \n";
		return;
	}
	music.play();
}

void Player::setConfig(Config* config) {
	this->config = config;
}

sf::Vector2f Player::getGravity() {
	return gravity;
}

bool Player::isLiving() {
	return isLive;
}

void Player::setLive(bool b) {
	isLive=b;
	if (!isLive) {
		music.pause();
		isLive = false;
		text.setString(std::to_string(deaths));
		deaths++;
	}
}

bool Player::getState() {
	return hasWon;
}

void Player::update(const float dt) {
	handleInput();
	updateMovement(dt);
}
