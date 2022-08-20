#include "EditorLauncher.h"
#include <filesystem>

void EditorLauncher::initVars() {
	view = config->window.getDefaultView();

	color1 = sf::Color::White;
	color2 = sf::Color(96, 96, 96);
	color3 = sf::Color::Black;

	bkg.setPrimitiveType(sf::Quads);
	bkg.resize(4);
	bkg[0] = sf::Vertex(sf::Vector2f(0, 0), color1);
	bkg[1] = sf::Vertex(sf::Vector2f(view.getSize().x, 0), color2);
	bkg[2] = sf::Vertex(sf::Vector2f(view.getSize().x, view.getSize().y), color3);
	bkg[3] = sf::Vertex(sf::Vector2f(0, view.getSize().y), color2);

	font.loadFromFile("Assets/Fonts/Roboto-Black.ttf");

	b_create.text.setFont(font);
	b_create.shape.setSize({300.f, 100.f});
	b_create.text.setString("Create new map");
	b_create.shape.setPosition({ (view.getSize().x - b_create.shape.getSize().x) / 2.f, 50.f});
	b_create.setOnPress([=]() {
		pendingstate = std::make_unique<Editor>(config);
	});

	std::filesystem::path p = std::filesystem::current_path() / "Assets/Mapdata";
	int width = view.getSize().x;

	int i = 0;
	for (const auto& entry : std::filesystem::directory_iterator(p)) {
		std::string mapname = entry.path().stem().string();

		Button button(
			{
				(view.getSize().x / 2) * (i%2) + 50.f * ( (i+1)%2),
				b_create.shape.getPosition().y + b_create.shape.getSize().y + (i/2) * 90.f + 50.f,
				560.f,
				80.f
			},
			mapname
		);

		button.text.setFont(font);

		button.setOnPress([=]() {
			pendingstate = std::make_unique<Editor>(config, mapname);
		});

		std::ifstream in("Assets/Mapdata/" + entry.path().filename().string());
		std::string color;
		in >> color;
		in.close();

		sf::Color difc;
		
		if (color == "green") difc = sf::Color::Green;
		else if (color == "red") difc = sf::Color::Red;
		else if (color == "cyan") difc = sf::Color::Cyan;
		else if (color == "yellow") difc = sf::Color::Yellow;
		else difc = sf::Color::Blue;

		button.ctext = sf::Color::White;
		button.cbkg = button.cbkghover = difc;
		button.cbkg.a = 128;
		button.cbkghover.a = 192;

		buttons.push_back(button);
		i++;
	}
}

void EditorLauncher::render() {
	config->window.setView(view);
	config->window.clear();

	config->window.draw(bkg);
	config->window.draw(b_create);

	for (const Button& button : buttons) {
		config->window.draw(button);
	}
}

void EditorLauncher::update(const float dt) {
	sf::Event event;
	while(config->window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			config->window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				shouldClose = true;
				return;
			}
			break;
		case sf::Event::Resized:
			resize(event.size.width, event.size.height);
			break;
		default:
			break;
		}
	}
	auto mouse = sf::Mouse::getPosition(config->window);
	sf::Vector2f worldPos = config->window.mapPixelToCoords(mouse);
	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	b_create.update(worldPos, mousePressed);
	for (Button& button : buttons) {
		button.update(worldPos, mousePressed);
	}

}

void EditorLauncher::resize(int width, int height) {
	view = sf::View(sf::FloatRect(0, 0, width, height));
	bkg[1] = sf::Vertex(sf::Vector2f(width, 0), color2);
	bkg[2] = sf::Vertex(sf::Vector2f(width, height), color3);
	bkg[3] = sf::Vertex(sf::Vector2f(0, height), color2);

	b_create.shape.setPosition({ (view.getSize().x - b_create.shape.getSize().x) / 2.f, 50.f });
}

EditorLauncher::EditorLauncher(Config* config) {
	this->config = config;

	initVars();
}

EditorLauncher::~EditorLauncher() { }
