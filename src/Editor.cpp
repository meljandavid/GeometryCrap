#include"Editor.h"

Editor::Editor(Config* config, const std::string& editfile) {
	filename = editfile;
	this->config = config;
	loadMap();
	initVars();
}

Editor::~Editor() {
	std::ofstream out("Assets/Mapdata/" + filename + ".txt");
	if (!out) {
		std::cout << "Failed to save the map\n";
		return;
	}

	out << musicname << std::endl << mapcolor << std::endl;

	out << lvl[0].size() << " " << lvl.size() << std::endl;
	for (size_t y = 0; y < lvl.size(); y++) {
		for (size_t x = 0; x < lvl[y].size(); x++)
			out << lvl[y][x].x << " ";
		if (y != lvl.size()) out << std::endl;
	}

	out << portals.size() << std::endl;
	for (const Portal& p : portals) {
		out << p.portals.first.x << " " << p.portals.first.y << " " <<
			p.portals.second.x << " " << p.portals.second.y << std::endl;
	}

	out.close();
}

void Editor::initVars() {
	WIDTH = config->window.getSize().x;
	HEIGHT = config->window.getSize().y;

	edit.reset(sf::FloatRect(0, 0, float(WIDTH), HEIGHT * (1 - split)));
	edit.setViewport(sf::FloatRect(0, split, 1, 1 - split));
	UI.reset(sf::FloatRect(0, 0, (float)WIDTH, HEIGHT * split));
	UI.setViewport(sf::FloatRect(0, 0, 1.f, split));

	shape.setSize(sf::Vector2f(tileSize, tileSize));
	shape.setOutlineColor(sf::Color::Black);

	mapBox = sf::Rect<float>(0., 0.f, (tileSize+1) * lvl[0].size(), (tileSize+1) * lvl.size());

	scrollSpeed = 500.f;

	if (!font.loadFromFile("Assets/Fonts/Roboto-Black.ttf"))
		std::cout << "Failed to load fonts\n";
	logText.setFont(font);
	logText.setFillColor(sf::Color::Black);
	sstream << std::fixed << std::setprecision(2) << zoom << 'x';
	logText.setString(sstream.str());
	sstream.str(std::string());
	logText.setCharacterSize(static_cast<unsigned int>(HEIGHT * split * 0.8f));

	if (!texture.loadFromFile("Assets/Textures/edittiles.png"))
		std::cout << "Failed to load the texture\n";
	else sprite.setTexture(texture);

	buttons.resize(4);
	buttons[0].text.setString("AddRow");
	buttons[0].setOnPress([=]() { selected = -1; });
	buttons[1].text.setString("DelRow");
	buttons[1].setOnPress([=]() { selected = -2; });
	buttons[2].text.setString("AddCol");
	buttons[2].setOnPress([=]() { selected = -3; });
	buttons[3].text.setString("DelCol");
	buttons[3].setOnPress([=]() { selected = -4; });
	for (int i = 0; i < 4; i++) {
		buttons[i].shape.setPosition({ 3 * i * ( tileSize + 2) + 170.f, 10.f });
		buttons[i].shape.setSize({ 3 * tileSize, HEIGHT * split - 20.f });
		buttons[i].text.setFont(font);
		buttons[i].text.setCharacterSize(unsigned int(HEIGHT * split / 3));
	}

	b_save.text.setString("Save it!");
	b_save.setOnPress([=]() {
		auto tfval = tf_save.getVal();
		if (tfval.has_value()) {
			filename = tfval.value();
			shouldClose = true;
		}
	});
	b_save.shape.setSize({ 3 * tileSize, HEIGHT * split - 20.f });
	b_save.shape.setPosition({ config->window.getSize().x - b_save.shape.getSize().x - 10.f, 10.f});
	b_save.text.setFont(font);
	b_save.text.setCharacterSize(unsigned int(HEIGHT * split / 3));

	tf_save.text.setFont(font);
	tf_save.text.setCharacterSize(unsigned int(HEIGHT * split / 3));
	tf_save.setTitle(filename);
	tf_save.setInitialText(filename);
	tf_save.shape.setSize({ 5 * tileSize, HEIGHT * split - 20.f });
	tf_save.shape.setPosition({ b_save.shape.getPosition().x - tf_save.shape.getSize().x - 10.f, 10.f});
	tf_save.setCharCheckerFunc([](char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
	});
}

void Editor::loadMap() {
	std::ifstream in("Assets/Mapdata/" + filename + ".txt");
	if (!in) {
		lvl.resize(5, std::vector<eTile>(10));
		musicname = "keppelskies_pablo4.ogg";
		mapcolor = "blue";
	}
	else {
		in >> musicname >> mapcolor;

		unsigned int w, h; in >> w >> h;
		lvl.resize(h, std::vector<eTile>(w));
		for (size_t y = 0; y < h; y++) {
			for (size_t x = 0; x < w; x++) {
				in >> lvl[y][x].x;
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
	config->window.setView(edit);
	v = sf::Vector2f(0, 0);
}

void Editor::handleEvents() {
	sf::Event event;
	while (config->window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			config->window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				shouldClose = true;
			break;
		case sf::Event::TextEntered:
			if (event.text.unicode < 128) {
				tf_save.textInput(static_cast<char>(event.text.unicode));
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (!mapBox.contains(worldPos) || event.mouseButton.button != sf::Mouse::Left) break;

			if (selected == -1) { // Add new ROW
				int ey = int((worldPos.y + tileSize / 2) / (tileSize + 1));
				int xd = lvl[0].size();
				for (Portal& p : portals) {
					if (p.portals.first.y >= ey) p.portals.first.y++;
					if (p.portals.second.y >= ey) p.portals.second.y++;
				}
				lvl.insert(lvl.begin() + ey, std::vector<eTile>(xd, {false, 0}));
			}
			else if (selected == -2) { // Delete ROW
				int ey = int((worldPos.y) / (tileSize + 1));
				
				for (auto it = portals.begin(); it != portals.end(); it++) {
					if (it->portals.first.y == ey || it->portals.second.y == ey) {
						lvl[it->portals.first.y][it->portals.first.x].x = 0;
						lvl[it->portals.second.y][it->portals.second.x].x = 0;
						portals.erase(it);
						it--;
					}
					else {
						if (it->portals.first.y > ey) it->portals.first.y--;
						if (it->portals.second.y > ey) it->portals.second.y--;
					}
				}

				lvl.erase(lvl.begin() + ey);
			}
			else if (selected == -3) { // Add new COLUMN
				int ex = int((worldPos.x + tileSize / 2) / (tileSize + 1));
				for (Portal& p : portals) {
					if (p.portals.first.x >= ex) p.portals.first.x++;
					if (p.portals.second.x >= ex) p.portals.second.x++;
				}
				for (std::vector<eTile>& row : lvl) {
					row.insert(row.begin() + ex, eTile{ false, 0 });
				}
			}
			else if (selected == -4) { // Delete COLUMN
				int ex = int((worldPos.x) / (tileSize + 1));
				
				for (auto it = portals.begin(); it != portals.end(); it++) {
					if (it->portals.first.x == ex || it->portals.second.x == ex) {
						lvl[it->portals.first.y][it->portals.first.x].x = 0;
						lvl[it->portals.second.y][it->portals.second.x].x = 0;
						portals.erase(it);
						it--;
					}
					else {
						if (it->portals.first.x > ex) it->portals.first.x--;
						if (it->portals.second.x > ex) it->portals.second.x--;
					}
				}

				for (std::vector<eTile>& row : lvl) {
					row.erase(row.begin() + ex);
				}
			}
			mapBox = sf::Rect<float>(0., 0.f, (tileSize + 1) * lvl[0].size(), (tileSize + 1) * lvl.size());
			break;
		case sf::Event::Resized:
			resize(event.size.width, event.size.height);
			break;
		case sf::Event::MouseWheelScrolled:
			zoomfactor = std::pow(.9f, event.mouseWheelScroll.delta);
			edit.zoom(zoomfactor);
			zoom *= zoomfactor;

			sstream << std::fixed << std::setprecision(2) << zoom << 'x';
			logText.setString(sstream.str());
			sstream.str(std::string());

			config->window.setView(edit);
			break;
		default:
			break;
		}
	}

	mouse = sf::Mouse::getPosition(config->window);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (textureSelectorBox.contains(sf::Vector2f(float(mouse.x), float(mouse.y)))) {
			selected = int(mouse.x - textureSelectorBox.left - 1) / textureSelectorBox.height;

			//PORTAL
			if (ids % 2 != 0) selected = 4;
		}
		if (mapBox.contains(worldPos)) {
			if (selected >= 0 && selected < 100) {
				sf::Vector2u modify = sf::Vector2u(static_cast<int>((worldPos.x) / (tileSize + 1)),
					static_cast<int>((worldPos.y) / (tileSize + 1)));
				eTile& t = lvl[modify.y][modify.x];

				//PORTAL
				if (selected == 4) {
					if (t.x == 4) goto out;
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
					for (auto it = portals.begin(); it != portals.end(); it++) {
						if (it->portals.first == modify) {
							lvl[it->portals.second.y][it->portals.second.x].x = 0;
							portals.erase(it);
							it--;
						} else if (it->portals.second == modify) {
							lvl[it->portals.first.y][it->portals.first.x].x = 0;
							portals.erase(it);
							it--;
						}
					}
				}

				t.x = selected;
			}
		}
	}

	out:

	config->window.setView(UI);
	sf::Vector2f worldPos = config->window.mapPixelToCoords(mouse);
	
	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	tf_save.update(worldPos, mousePressed);
	b_save.update(worldPos, mousePressed);
	for (Button& button : buttons) {
		button.update(worldPos, mousePressed);
	}
}

void Editor::render() {
	config->window.clear(sf::Color::White);

	config->window.setView(edit);
	shape.setSize(sf::Vector2f(tileSize, tileSize));
	for (size_t y = 0; y < lvl.size(); y++) {
		for (size_t x = 0; x < lvl[y].size(); x++) {
			sprite.setPosition(sf::Vector2f(float(x), float(y)) * (tileSize + 1));
			if (lvl[y][x].highlight) {
				shape.setPosition(sf::Vector2f(float(x), float(y)) * (tileSize + 1));
				shape.setOutlineThickness(1);
				lvl[y][x].highlight = false;
				config->window.draw(shape);
				shape.setOutlineThickness(0);
			}

			sprite.setTextureRect(sf::IntRect(lvl[y][x].x * int(tileSize),0, int(tileSize), int(tileSize)));
			config->window.draw(sprite);
		}
	}

	config->window.setView(UI);
	for (Button& b : buttons) {
		config->window.draw(b);
	}
	config->window.draw(b_save);
	config->window.draw(tf_save);
	config->window.draw(logText);

	sprite.setTextureRect(sf::IntRect(0.f, 0.f, int(texture.getSize().x), int(texture.getSize().y)));
	sprite.setPosition(sf::Vector2f(buttons.back().shape.getPosition().x + buttons.back().shape.getSize().x + tileSize, 10.f));
	float scl = buttons[0].shape.getSize().y / sprite.getTextureRect().height;
	sprite.setScale(sf::Vector2f(scl, scl));
	textureSelectorBox = sprite.getGlobalBounds();
	config->window.draw(sprite);
	sprite.setScale(sf::Vector2f(1.f, 1.f));

	
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(5.f);
	shape.setOutlineColor(sf::Color::Red);
	
	if (selected >= 0 && selected < 100) {
		shape.setSize(sf::Vector2f((float)textureSelectorBox.height, (float)textureSelectorBox.height));
		shape.setPosition({ float(textureSelectorBox.left + selected * textureSelectorBox.height),
			(float)textureSelectorBox.top });
	}
	else {
		const Button& b = buttons[-selected - 1];
		shape.setSize(b.shape.getSize());
		shape.setPosition(b.shape.getPosition());
	}
	config->window.draw(shape);

	shape.setOutlineThickness(0.f);
	if(selected < 0.f) {
		config->window.setView(edit);
		mouse = sf::Mouse::getPosition(config->window);
		worldPos = config->window.mapPixelToCoords(mouse);
		if (mapBox.contains(worldPos)) {
			float shift = 0.5f;
			if (selected % 2 == 0) {
				shape.setFillColor(sf::Color(255, 0, 0, 192));
				shift = 0.f;
			}
			else shape.setFillColor(sf::Color(0,255,0,192));

			if (selected >= -2) {
				shape.setSize({ edit.getSize().x, tileSize});
				int ey = int((worldPos.y + tileSize*shift) / (tileSize + 1));
				shape.setPosition({ edit.getCenter().x - edit.getSize().x/2, (tileSize + 1) * (ey - shift)});
			}
			else {
				shape.setSize({ tileSize, edit.getSize().y });
				int ex = int((worldPos.x + tileSize*shift) / (tileSize + 1));
				shape.setPosition({(tileSize + 1) * (ex - shift), edit.getCenter().y - edit.getSize().y / 2 });
			}
			
			config->window.draw(shape);
		}
	}
	
	shape.setOutlineThickness(0);
	shape.setOutlineColor(sf::Color::Black);
}

void Editor::update(const float _dt) {
	dt = _dt;
	updateView();

	mouse = sf::Mouse::getPosition(config->window);
	worldPos = config->window.mapPixelToCoords(mouse);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		if (rmhold) {
			sf::Vector2f v = {float(holdmouse.x-mouse.x), float(holdmouse.y-mouse.y)};
			v /= float(1/zoom);
			edit.move(v);
			config->window.setView(edit);
		}
		rmhold = true;
	}
	else rmhold = true;
	holdmouse = mouse;

	handleEvents();

	if (mapBox.contains(worldPos)) {
		lvl[static_cast<int>((worldPos.y) / (tileSize+1))][static_cast<int>((worldPos.x) / (tileSize+1))].highlight = true;
	}
}

void Editor::resize(int width, int height) {
	logText.setCharacterSize(static_cast<unsigned int>(height * split * 0.8f));
	UI.setSize((float)width, height * split);
	UI.setCenter(sf::Vector2f(width / 2.f, height * split / 2));
	edit.setSize((float)width, height * (1 - split));
	auto charsize = unsigned int(height * split / 3);
	for (int i = 0; i < 4; i++) {
		buttons[i].shape.setSize({ 3 * tileSize, height * split - 20.f });
		buttons[i].text.setCharacterSize(charsize);
		buttons[i].shape.setPosition({
			3 * i * (tileSize + 2) + logText.getLocalBounds().width + 20.f,
			10.f });
	}

	b_save.text.setCharacterSize(charsize);
	b_save.shape.setSize({ 3 * tileSize, height * split - 20.f });
	b_save.shape.setPosition({ config->window.getSize().x - b_save.shape.getSize().x - 10.f, 10.f });

	tf_save.text.setCharacterSize(charsize);
	tf_save.shape.setSize({ 5 * tileSize, height * split - 20.f });
	tf_save.shape.setPosition({ b_save.shape.getPosition().x - tf_save.shape.getSize().x - 10.f, 10.f });
}
