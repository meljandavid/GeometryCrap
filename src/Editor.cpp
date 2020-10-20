#include"Editor.h"

Editor::Editor(Base* game) {
	this->game = game;
	loadMap();
	initVars();
}

Editor::~Editor() {
	out << lvl[0].size() << " " << lvl.size() << std::endl;
	for (size_t y = 0; y < lvl.size(); y++) {
		for (size_t x = 0; x < lvl[y].size(); x++)
			out << lvl[y][x].x << " ";
		if (y != lvl.size()) out << std::endl;
	}
	/*display portals
	for (const Portal::Editable& p : portals) {
		std::cout << "ID: " << p.id << " || start: " << p.portals.first.x << "/" << p.portals.first.y
			<< " || end: " << p.portals.second.x << "/" << p.portals.second.y << std::endl;
	}*/
	out << portals.size() << std::endl;
	for (const Portal& p : portals) {
		out << p.portals.first.x << " " << p.portals.first.y << " " <<
			p.portals.second.x << " " << p.portals.second.y << std::endl;
	}

	out.close();
}

void Editor::initVars() {
	out.open("Assets/mymap.txt");
	if (!out) {
		std::cout << "Failed to load the map\n";
		return;
	}

	WIDTH = game->window.getSize().x;
	HEIGHT = game->window.getSize().y;

	edit.reset(sf::FloatRect(0, 0, float(WIDTH), HEIGHT * (1 - split)));
	edit.setViewport(sf::FloatRect(0, split, 1, 1 - split));
	UI.reset(sf::FloatRect(0, 0, (float)WIDTH, HEIGHT * split));
	UI.setViewport(sf::FloatRect(0, 0, 1.f, split));

	shape.setSize(sf::Vector2f(tileSize, tileSize));
	shape.setOutlineColor(sf::Color::Black);

	offset = sf::Vector2f(0., 0.);
	mapBox = sf::Rect<float>(offset.x, offset.y,
		tileSize * lvl[0].size(), tileSize * lvl.size());

	scrollSpeed = 500.f;

	if (!font.loadFromFile("Assets/Roboto-Black.ttf"))
		std::cout << "Failed to load fonts\n";
	logText.setFont(font);
	logText.setFillColor(sf::Color::Black);
	sstream << std::fixed << std::setprecision(2) << zoom << 'x';
	logText.setString(sstream.str());
	sstream.str(std::string());
	logText.setCharacterSize(static_cast<unsigned int>(HEIGHT * split * 0.8f));

	if (!texture.loadFromFile("Assets/edittiles.png"))
		std::cout << "Failed to load the texture\n";
	else sprite.setTexture(texture);

	buttons.resize(4);
	buttons[0].text = sf::Text("+X", font, static_cast<unsigned int>(HEIGHT * split / 2.f));
	buttons[1].text = sf::Text("-X", font, static_cast<unsigned int>(HEIGHT * split / 2.f));
	buttons[2].text = sf::Text("+Y", font, static_cast<unsigned int>(HEIGHT * split / 2.f));
	buttons[3].text = sf::Text("-Y", font, static_cast<unsigned int>(HEIGHT * split / 2.f));
	for (int i = 0; i < 4; i++) {
		buttons[i].hitbox = sf::FloatRect(200.f + (2*tileSize + 5.f) * i, 10.f, 2*tileSize, HEIGHT * split - 20.f);
		buttons[i].text.setFillColor(sf::Color::Black);
		buttons[i].text.setPosition(sf::Vector2f(buttons[i].hitbox.left+10.f, buttons[i].hitbox.top));
	}
}

void Editor::loadMap() {
	std::ifstream in("Assets/mymap.txt");
	if (!in) {
		lvl.resize(5, std::vector<eTile>(10));
	}
	else {
		unsigned int w, h; in >> w >> h;
		lvl.resize(h, std::vector<eTile>(w));
		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				in >> lvl[y][x].x;
			}
		}
	}
	int ps; in >> ps;
	portals.resize(ps);

	for (Portal& p : portals) {
		int x, y; in >> x >> y;
		p.portals.first.x = x;
		p.portals.first.y = y;
		in >> x >> y;
		p.portals.second.x = x;
		p.portals.second.y = y;
	}

	in.close();
}

void Editor::updateView() {
	sf::Vector2f v;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) v.x = -scrollSpeed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) v.x = scrollSpeed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) v.y = -scrollSpeed * dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) v.y = scrollSpeed * dt;
	edit.move(v);
	game->window.setView(edit);
	v = sf::Vector2f(0, 0);
}

void Editor::handleEvents() {
	sf::Event event;
	while (game->window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			game->window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				rip = true;
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::LShift) {
				edit.zoom(0.8f);
				zoom /= .8f;

				sstream << std::fixed << std::setprecision(2) << zoom << 'x';
				logText.setString(sstream.str());
				sstream.str(std::string());

				game->window.setView(edit);
			}
			else if (event.key.code == sf::Keyboard::LControl) {
				edit.zoom(1.25f);
				zoom /= 1.25f;

				sstream << std::fixed << std::setprecision(2) << zoom << 'x';
				logText.setString(sstream.str());
				sstream.str(std::string());

				game->window.setView(edit);
			}
			break;
		case sf::Event::MouseButtonPressed:
			for (size_t i = 0; i < buttons.size(); i++) {
				if (buttons[i].hitbox.contains(sf::Vector2f(float(mouse.x), float(mouse.y)))) {
					switch (i) {
					case 0:
						modifyMap(1, 0);
						break;
					case 1:
						modifyMap(-1, 0);
						break;
					case 2:
						modifyMap(0, 1);
						break;
					case 3:
						modifyMap(0, -1);
						break;
					default:
						break;
					}
					goto out;
				}
			}
			if (textureSelectorBox.contains(sf::Vector2f(float(mouse.x), float(mouse.y)))) {
				selectedTexture = int(mouse.x - textureSelectorBox.left - 1) / textureSelectorBox.height;

				//PORTAL
				if (ids % 2 != 0) selectedTexture = 4;

				goto out;
			}
			if(mapBox.contains(worldPos)) {
				sf::Vector2u modify = sf::Vector2u(static_cast<int>((worldPos.x - offset.x) / (tileSize + 1)),
					static_cast<int>((worldPos.y - offset.y) / (tileSize + 1)));
				eTile& t = lvl[modify.y][modify.x];

				//PORTAL
				if (selectedTexture == 4) {
					if (ids % 2 == 0) {
						portals.push_back(Portal());
						portals.back().portals.first = modify;
					}
					else {
						portals.back().portals.second = modify;
					}

					ids++;
				}
				else if (t.x == 4) {
					for (int i = 0; i < portals.size(); i++) {
						if (portals[i].portals.first == modify) {
							lvl[portals[i].portals.second.y][portals[i].portals.second.x].x = 0;
							portals.erase(portals.begin() + i);
							break;
						}
						if (portals[i].portals.second == modify) {
							lvl[portals[i].portals.first.y][portals[i].portals.first.x].x = 0;
							portals.erase(portals.begin() + i);
							break;
						}
					}
				}

				t.x = selectedTexture;
			}

			out:
			break;
		case sf::Event::Resized:
			logText.setCharacterSize(static_cast<unsigned int>(event.size.height * split * 0.8f));
			UI.setSize((float)event.size.width, event.size.height * split);
			UI.setCenter(sf::Vector2f(event.size.width / 2.f, event.size.height * split / 2));
			edit.setSize((float)event.size.width, event.size.height * (1 - split));
			for (int i = 0; i < 4; i++) {
				buttons[i].hitbox.height = event.size.height * split * 0.8f;
				buttons[i].hitbox.width = event.size.height * split;
				buttons[i].hitbox.left = event.size.height * split * 3.f + i * (buttons[i].hitbox.width + 5.f);
				buttons[i].text.setCharacterSize(static_cast<int>(event.size.height * split / 2.f));
				buttons[i].text.setPosition(sf::Vector2f(buttons[i].hitbox.left + buttons[i].hitbox.width/5.f, buttons[i].hitbox.top));
			}
			break;
		default:
			break;
		}
	}
}

void Editor::modifyMap(int horizontal, int vertical) {
	int currY = lvl.size(), currX = lvl[0].size();

	if (currY + vertical > 0) {
		lvl.resize(currY + vertical, std::vector<eTile>(currX));
		mapBox.height = tileSize * (currY + vertical);
	}
	if (currX + horizontal > 0) {
		for (std::vector<eTile>& vt : lvl) {
			vt.resize(currX + horizontal);
		}
		mapBox.width = tileSize * (currX + horizontal);
	}
}

void Editor::render() {
	game->window.clear(sf::Color::White);

	game->window.setView(edit);
	shape.setSize(sf::Vector2f(tileSize, tileSize));
	for (size_t y = 0; y < lvl.size(); y++) {
		for (size_t x = 0; x < lvl[y].size(); x++) {
			sprite.setPosition(sf::Vector2f(float(x), float(y)) * (tileSize + 1) + offset);
			if (lvl[y][x].highlight) {
				shape.setPosition(sf::Vector2f(float(x), float(y)) * (tileSize + 1) + offset);
				shape.setOutlineThickness(1);
				lvl[y][x].highlight = false;
				game->window.draw(shape);
				shape.setOutlineThickness(0);
			}

			sprite.setTextureRect(sf::IntRect(lvl[y][x].x * int(tileSize),0, int(tileSize), int(tileSize)));
			game->window.draw(sprite);
		}
	}

	game->window.setView(UI);
	for (Button& b : buttons) {
		shape.setPosition(sf::Vector2f(b.hitbox.left, b.hitbox.top));
		shape.setSize(sf::Vector2f(b.hitbox.width, b.hitbox.height));
		shape.setFillColor(sf::Color::Yellow);
		if (b.hitbox.contains(sf::Vector2f(float(mouse.x), float(mouse.y)))) {
			shape.setOutlineThickness(2.f);
			shape.setOutlineColor(sf::Color::Black);
		}
		game->window.draw(shape);
		game->window.draw(b.text);

		shape.setOutlineThickness(0);
	}
	game->window.draw(logText);

	sprite.setTextureRect(sf::IntRect(0.f, 0.f, int(texture.getSize().x), int(texture.getSize().y)));
	sprite.setPosition(sf::Vector2f(buttons[3].hitbox.left + buttons[3].hitbox.width + 100.f,10.f));
	float scl = (float)buttons[0].hitbox.height / sprite.getTextureRect().height;
	sprite.setScale(sf::Vector2f(scl, scl));
	textureSelectorBox = sprite.getGlobalBounds();
	game->window.draw(sprite);
	sprite.setScale(sf::Vector2f(1.f, 1.f));

	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(5.f);
	shape.setOutlineColor(sf::Color::Red);
	shape.setPosition(sf::Vector2f(float(textureSelectorBox.left + selectedTexture * textureSelectorBox.height), (float)textureSelectorBox.top));
	shape.setSize(sf::Vector2f((float)textureSelectorBox.height, (float)textureSelectorBox.height));
	game->window.draw(shape);
	shape.setOutlineThickness(0);
	shape.setOutlineColor(sf::Color::Black);

	if (rip) game->popState();
}

void Editor::update(const float _dt) {
	dt = _dt;
	updateView();

	mouse = sf::Mouse::getPosition(game->window);
	worldPos = game->window.mapPixelToCoords(mouse);

	handleEvents();

	if (mapBox.contains(worldPos)) {
		lvl[static_cast<int>((worldPos.y - offset.y) / (tileSize+1))][static_cast<int>((worldPos.x - offset.x) / (tileSize+1))].highlight = true;
	}
}
