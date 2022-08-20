#include"Game.h"

Game::Game(Config* config, const std::string &s) {
	this->config = config;

	file = "Assets/Mapdata/" + s + ".txt";
	initVars();
}

Game::~Game() {
}

void Game::initVars() {
	tileSize = 32.f;

	std::ifstream in(file);

	if (!in) std::cerr << "FAILed to open " << file << std::endl;
	else {
		in >> bkgcolor >> musicname;
		int w, h;
		in >> w >> h;
		level.resize(h, std::vector<int>(w));
		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				in >> level[y][x];
				if (level[y][x] == 5) {
					turrets.push_back(Turret(sf::Vector2f(x * tileSize, y * tileSize)));
				}
			}
		}

		int ps; in >> ps;
		portals.resize(ps);

		for (Portal& p : portals) {
			int x, y;
			in >> x >> y;
			p.portals.first.x = x;
			p.portals.first.y = y;
			in >> x >> y;
			p.portals.second.x = x;
			p.portals.second.y = y;
		}

		Log log;
	}

	in.close();

	player.setMusic(musicname);

	verticemap.load("Assets/Textures/tilesheet.png", sf::Vector2u(unsigned int(tileSize), unsigned int(tileSize)), level);
	player.setLevel(&level);
	player.setConfig(config);

	view.setSize(12 * tileSize / config->window.getSize().y * config->window.getSize().x, 12 * tileSize);
	view.setCenter(sf::Vector2f(verticemap.height * 16.f / config->window.getSize().y * config->window.getSize().x,
		verticemap.height * tileSize / 2));
	config->window.setView(view);
	ui = config->window.getDefaultView();

	bkg.setPrimitiveType(sf::Quads);
	bkg.resize(4);

	if (bkgcolor == "green") cbottom = sf::Color::Green;
	else if (bkgcolor == "red") cbottom = sf::Color::Red;
	else if (bkgcolor == "cyan") cbottom = sf::Color::Cyan;
	else if (bkgcolor == "yellow") cbottom = sf::Color::Yellow;
	else cbottom = sf::Color::Blue;

	bkg[0].color = cbottom;
	bkg[1].color = cbottom;
	bkg[2].color = ctop;
	bkg[3].color = ctop;
}

void Game::handleEvents() {
	sf::Event event;
	while (config->window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			config->window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				shouldClose = true;
				return;
			}
			if (event.key.code == sf::Keyboard::Space)
				player.Jump();
			break;
		case sf::Event::Resized:
			resize(event.size.width, event.size.height);
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Space)
				player.jumpReady = true;
			break;
		default:
			break;
		}
	}
}

void Game::update(const float dt) {
	handleEvents();

	if (player.isLiving()) {
		player.update(dt);

		for (int i = 0; i < turrets.size(); i++) {
			turrets[i].update(dt);
			int tX = (int)turrets[i].getBulletPos().x / tileSize;
			int tY = (int)turrets[i].getBulletPos().y / tileSize;
			if (tX < 0 || tileArr[level[tY][tX]].isCollidable) turrets[i].reset();

			if (player.shape.getGlobalBounds().contains(turrets[i].getBulletPos())) player.setLive(false);
			if (player.shape.getGlobalBounds().left > turrets[i].getPos().x) turrets[i].setActive(false);
		}
		for (Portal& p : portals) {
			sf::FloatRect hb(tileSize * p.portals.first.x, tileSize * p.portals.first.y,
				tileSize, tileSize);
			if (hb.intersects(player.shape.getGlobalBounds()) && !p.used) {
				player.shape.setPosition(tileSize * sf::Vector2f(p.portals.second.x, p.portals.second.y));
				p.used = true;
			}

			hb.left = tileSize * p.portals.second.x;
			hb.top = tileSize * p.portals.second.y;
			if (hb.intersects(player.shape.getGlobalBounds()) && !p.used) {
				player.shape.setPosition(tileSize * sf::Vector2f(p.portals.first.x, p.portals.first.y));
				p.used = true;
			}
		}
	}
	else if(!player.getState()) {
		respawn += dt;
		if (respawn >= 1.f) {
			player.reset();
			for (Turret& t : turrets) {
				t.reset();
			}
			for (Portal& p : portals) { p.used = false; }
			respawn = 0.f;
		}
	}
}

void Game::render() {
	config->window.clear(sf::Color::Black);

	config->window.setView(view);

	//background
	float left = view.getCenter().x - view.getSize().x / 2;
	float top = view.getCenter().y - view.getSize().y / 2;
	bkg[0].position = sf::Vector2f(left, top);
	bkg[1].position = sf::Vector2f(left + view.getSize().x, top);
	bkg[2].position = sf::Vector2f(left + view.getSize().x, top + view.getSize().y);
	bkg[3].position = sf::Vector2f(left, top + view.getSize().y);
	config->window.draw(bkg);

	player.render(&(config->window));
	config->window.draw(verticemap);
	for (Turret& t : turrets) {
		t.render(&(config->window));
	}
	player.renderUI(&(config->window));
	view = config->window.getView();

	config->window.setView(ui);
}

void Game::resize(int width, int height) {
	view.setSize(12 * tileSize / height * width, 12 * tileSize);
	ui.setSize(width, height);
	config->window.setView(view);
}
