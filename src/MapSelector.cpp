#include "MapSelector.h"

void MapSelector::moveLeft() {
    nextmap--;
    vel = maps[nextmap].shape.getPosition()
        - maps[currmap].shape.getPosition();
    vel /= 2.f; // yo wtf?

    colordelta = {
                float(maps[nextmap].color.r - maps[currmap].color.r),
                float(maps[nextmap].color.g - maps[currmap].color.g),
                float(maps[nextmap].color.b - maps[currmap].color.b)
    };
}

void MapSelector::moveRight() {
    nextmap++;
    vel = maps[nextmap].shape.getPosition()
        - maps[currmap].shape.getPosition();
    vel /= 2.f; // wish i knew why is this correct

    colordelta = {
                float(maps[nextmap].color.r - maps[currmap].color.r),
                float(maps[nextmap].color.g - maps[currmap].color.g),
                float(maps[nextmap].color.b - maps[currmap].color.b)
    };
}

void MapSelector::genBkgColors() {
    color1 = sf::Color((255.f+R)/2.f, (255.f + G) / 2.f, (255.f + B) / 2.f);
    color2 = sf::Color(R, G, B);
    color3 = sf::Color(R * .2f, G * .2f, B * .2f);
}

void MapSelector::clamp(float& f) {
    if (f < 0.f) f = 0.f;
    if (f > 255.f) f = 255.f;
}

void MapSelector::initVars() {
    view = config->window.getDefaultView();

    font.loadFromFile("Assets/Fonts/FoughtKnight.ttf");

    cardsize = view.getSize() * .6f;

    std::filesystem::path p = std::filesystem::current_path() / "Assets/Mapdata";

    int i = 0;
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        std::string mapname = entry.path().stem().string();

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

        if (i == 0) {
            R = difc.r;
            G = difc.g;
            B = difc.b;
        }

        Mapcard mc;
        mc.color = difc;
        mc.shape.setSize(cardsize);
        mc.shape.setPosition({
            (i * 0.7f + 0.2f) * view.getSize().x,
            (view.getSize().y - mc.shape.getSize().y) / 2.f
        });
        mc.shape.setOutlineThickness(5.f);
        mc.shape.setFillColor(sf::Color(255, 255, 255, 64));
        mc.text.setFont(font);
        mc.text.setFillColor(sf::Color::Black);
        mc.text.setCharacterSize(120.f);
        mc.text.setString(mapname);
        mc.text.setPosition({
            mc.shape.getPosition().x + (mc.shape.getSize().x - mc.text.getLocalBounds().width) / 2.f,
            mc.shape.getPosition().y + (mc.shape.getSize().y - mc.text.getLocalBounds().height) / 2.f
        });

        maps.push_back(mc);

        i++;
    }

    bkg.setPrimitiveType(sf::Quads);
    bkg.resize(4);

    genBkgColors();
}

void MapSelector::render() {
    config->window.setView(view);
    config->window.clear();

    config->window.draw(bkg);

    for (const Mapcard& mapcard : maps) {
        config->window.draw(mapcard.shape);
        config->window.draw(mapcard.text);
    }
}

void MapSelector::update(const float dt) {
	mouse = sf::Mouse::getPosition(config->window);
	sf::Vector2f worldPos = config->window.mapPixelToCoords(mouse);
    bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (music->getStatus() != sf::Music::Playing) {
        view.setCenter(view.getSize() / 2.f);
        currmap = nextmap = 0;
        music->play();
    }
    if (music->getVolume() < config->volume * 100.f) {
        music->setVolume(std::min(music->getVolume() + dt * 50.f, config->volume * 100.f));
    }

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
            break;
        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Left) {
                if (currmap > 0 && transition == 0.f) {
                    moveLeft();
                }
            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (currmap < maps.size() - 1 && transition == 0.f) {
                    moveRight();
                }
            }

            break;
        case sf::Event::Resized:
            resize(event.size.width, event.size.height);
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < maps.size(); i++) {
        maps[i].shape.setFillColor(sf::Color(255, 255, 255, 64));
        if (maps[i].shape.getGlobalBounds().contains(worldPos)) {
            maps[i].shape.setFillColor(sf::Color(255, 255, 255, 96));
            if (mousePressed) {
                maps[i].shape.setFillColor(sf::Color(255, 255, 255, 160));
                if (i == currmap) {
                    pendingstate = std::make_unique<Game>(config, maps[currmap].text.getString());
                    music->pause();
                }
                else if (i == currmap - 1 && transition == 0.f) {
                    moveLeft();
                }
                else if (i == currmap + 1 && transition == 0.f) {
                    moveRight();
                }
            }
        }
    }

    float left = view.getCenter().x - view.getSize().x / 2;
    float top = view.getCenter().y - view.getSize().y / 2;
    bkg.clear();
    bkg.resize(4);
    genBkgColors();
    bkg[0] = sf::Vertex(sf::Vector2f(left, top), color1);
    bkg[1] = sf::Vertex(sf::Vector2f(view.getSize().x + left, top), color2);
    bkg[2] = sf::Vertex(sf::Vector2f(view.getSize().x + left, view.getSize().y + top), color2);
    bkg[3] = sf::Vertex(sf::Vector2f(left, view.getSize().y + top), color3);

    if (currmap != nextmap) {
        view.move(vel * (dt / t_transition));
        view.move(vel * (dt / t_transition));
        
        R += colordelta.x * (dt / t_transition);
        clamp(R);
        G += colordelta.y * (dt / t_transition);
        clamp(G);
        B += colordelta.z * (dt / t_transition);
        clamp(B);

        transition += dt;
        if (std::abs(t_transition - transition) < 0.001f) {
            currmap = nextmap;
            transition = 0.f;
        }
    }
}

void MapSelector::resize(int width, int height) {
    view = sf::View({ 0.f,0.f,float(width), float(height) });
    currmap = nextmap = 0;

    std::filesystem::path p = std::filesystem::current_path() / "Assets/Mapdata";

    cardsize = view.getSize() * .6f;

    maps.clear();
    int i = 0;
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        std::string mapname = entry.path().stem().string();

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

        if (i == 0) {
            R = difc.r;
            G = difc.g;
            B = difc.b;
        }

        Mapcard mc;
        mc.color = difc;
        mc.shape.setSize(cardsize);
        mc.shape.setPosition({
            (i * 0.7f + 0.2f) * view.getSize().x,
            (view.getSize().y - mc.shape.getSize().y) / 2.f
            });
        mc.shape.setOutlineThickness(5.f);
        mc.shape.setFillColor(sf::Color(255, 255, 255, 64));
        mc.text.setFont(font);
        mc.text.setFillColor(sf::Color::Black);
        mc.text.setCharacterSize(120.f);
        mc.text.setString(mapname);
        mc.text.setPosition({
            mc.shape.getPosition().x + (mc.shape.getSize().x - mc.text.getLocalBounds().width) / 2.f,
            mc.shape.getPosition().y + (mc.shape.getSize().y - mc.text.getLocalBounds().height) / 2.f
        });

        maps.push_back(mc);

        i++;
    }
}

MapSelector::MapSelector(Config* config, sf::Music* music) {
    this->music = music;
	this->config = config;
	initVars();
}

MapSelector::~MapSelector() {
}
