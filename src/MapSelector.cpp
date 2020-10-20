#include "MapSelector.h"

void MapSelector::initVars() {
    b_mymap.box= sf::FloatRect(10.f, 10.f, 400.f, 200.f);
    b_mymap.id = "mymap";
    shape.setOutlineColor(sf::Color::Black);

    font.loadFromFile("Assets/Dragz.otf");
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    shape.setFillColor(b_mymap.bkg);
    shape.setOutlineColor(sf::Color::Black);

    color1 = sf::Color::White;
    color2 = sf::Color(96, 96, 96);
    color3 = sf::Color::Black;

    bkg.setPrimitiveType(sf::Quads);
    bkg.resize(4);
    bkg[0] = sf::Vertex(sf::Vector2f(0, 0), color1);
    bkg[1] = sf::Vertex(sf::Vector2f(game->window.getSize().x, 0), color2);
    bkg[2] = sf::Vertex(sf::Vector2f(game->window.getSize().x, game->window.getSize().y), color3);
    bkg[3] = sf::Vertex(sf::Vector2f(0, game->window.getSize().y), color2);

    view = game->window.getDefaultView();

    in.open("Assets/settings.txt");
    if (!in) std::cout << "failed to open the settings file\n";
    else {
        int len=0; in >> len;
        buttons.resize(len);
        int width = view.getSize().x;
        for (int i = 0; i < len; i++) {
            in >> buttons[i].id;
            buttons[i].box = sf::FloatRect(sf::Vector2f(i % (width/400) * 425.f + 10.f,
                i / (width / 400) * 210.f + 300.f), sf::Vector2f(400.f, 200.f));
        }
    }
}

void MapSelector::render() {
    game->window.setView(view);
    game->window.clear();

    game->window.draw(bkg);

    shape.setFillColor(b_mymap.color);
    shape.setOutlineThickness(b_mymap.outline);
    shape.setSize(sf::Vector2f((float)b_mymap.box.width, (float)b_mymap.box.height));
    shape.setPosition(sf::Vector2f((float)b_mymap.box.left, (float)b_mymap.box.top));
    game->window.draw(shape);
    text.setString(b_mymap.id);
    text.setFillColor(b_mymap.textCol);
    text.setPosition(sf::Vector2f((float)b_mymap.box.left + b_mymap.box.width / 2.f - text.getGlobalBounds().width / 2,
        (float)b_mymap.box.top + b_mymap.box.height / 2.f - text.getGlobalBounds().height / 2));
    game->window.draw(text);

    for (const lButton& b : buttons) {
        shape.setSize(sf::Vector2f((float)b.box.width, (float)b.box.height));
        shape.setPosition(sf::Vector2f((float)b.box.left, (float)b.box.top));
        shape.setFillColor(b.color);
        shape.setOutlineThickness(b.outline);
        game->window.draw(shape);

        text.setString(b.id);
        text.setFillColor(b.textCol);
        text.setPosition(sf::Vector2f((float)b.box.left + b.box.width / 2.f - text.getGlobalBounds().width / 2,
            (float)b.box.top + b.box.height / 2.f - text.getGlobalBounds().height / 2));
        game->window.draw(text);
    }

    if (rip) game->popState();
}

void MapSelector::update(const float dt) {
	sf::Event event;

	mouse = sf::Mouse::getPosition(game->window);
	sf::Vector2f worldPos = game->window.mapPixelToCoords(mouse);

    while (game->window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            game->window.close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                rip = true;
                return;
            }
            break;
        case sf::Event::Resized:
            view = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
            bkg[1] = sf::Vertex(sf::Vector2f(view.getSize().x, 0), color2);
            bkg[2] = sf::Vertex(sf::Vector2f(view.getSize().x, view.getSize().y), color3);
            bkg[3] = sf::Vertex(sf::Vector2f(0, view.getSize().y), color2);
            for (int i = 0; i < buttons.size(); i++) {
                in >> buttons[i].id;
                buttons[i].box = sf::FloatRect(sf::Vector2f(i % (int(view.getSize().x) / 400) * 425.f + 10.f,
                    i / (int(view.getSize().x) / 400) * 210.f + 300.f), sf::Vector2f(400.f, 200.f));
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (b_mymap.box.contains(worldPos))
                game->pushState(new Game(game));
            for (lButton& b : buttons)
                if(b.box.contains(worldPos))
                    game->pushState(new Game(game, "Assets/"+ b.id+".txt"));
            break;
        default:
            break;
        }
    }

    if (b_mymap.box.contains(worldPos)) {
        b_mymap.outline=5.f;
        b_mymap.color = b_mymap.hover;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            b_mymap.color=b_mymap.onPress;
    }
    else {
        b_mymap.outline = 1.f;
        b_mymap.color = b_mymap.bkg;
    }

    for (lButton& b : buttons) {
        if (b.box.contains(worldPos)) {
            b.outline = 5.f;
            b.color = b.hover;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                b.color=b.onPress;
        }
        else {
            b.outline = 1.f;
            b.color = b.bkg;
        }
    }
}

MapSelector::MapSelector(Base* game) {
	this->game = game;
	initVars();
}

MapSelector::~MapSelector() {
}
