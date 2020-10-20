#include"SFML/Graphics.hpp"

#include "MainMenu.h"
#include "MapSelector.h"
#include "Runnable.h"

void MainMenu::render() {
    game->window.setView(view);

    game->window.clear();

    game->window.draw(bkg);
    game->window.draw(sprite);
    game->window.draw(editorText);
    game->window.draw(gameText);
}

void MainMenu::initVars() {
    view = game->window.getDefaultView();

    font.loadFromFile("Assets/Dragz.otf");

    editorText.setFont(font);
    editorText.setString("{EDITOR}");
    editorText.setPosition(sf::Vector2f(game->window.getSize().x / 2.25, game->window.getSize().y / 3));
    editorText.setCharacterSize(minSize);
    gameText.setFont(font);
    gameText.setString("{PLAY}");
    gameText.setPosition(sf::Vector2f(game->window.getSize().x / 2.25, game->window.getSize().y / 2));
    gameText.setCharacterSize(minSize);

    minSize = game->window.getSize().y / 10;
    maxSize = minSize + 1.f;

    if (!music.openFromFile("Assets/ks_fiberman.ogg")) {
        std::cout << "rip music :( \n";
        return;
    }
    music.setPlayingOffset(sf::seconds(0));
    music.play();
    music.setLoop(true);

    color1 = sf::Color::White;
    color2 = sf::Color(96, 96, 96);
    color3 = sf::Color::Black;

    bkg.setPrimitiveType(sf::Quads);
    bkg.resize(4);
    bkg[0] = sf::Vertex(sf::Vector2f(0, 0), color1);
    bkg[1] = sf::Vertex(sf::Vector2f(game->window.getSize().x, 0), color2);
    bkg[2] = sf::Vertex(sf::Vector2f(game->window.getSize().x, game->window.getSize().y), color3);
    bkg[3] = sf::Vertex(sf::Vector2f(0, game->window.getSize().y), color2);

    texture.loadFromFile("Assets/logoM.png");
    texture.setSmooth(true);
    sprite.setTexture(texture);
    scl = game->window.getSize().y / 2 / sprite.getGlobalBounds().height;
    sprite.setScale(sf::Vector2f(scl, scl));
    sprite.setPosition(sf::Vector2f(game->window.getSize().x / 10, game->window.getSize().y / 2 - sprite.getGlobalBounds().height / 2));
}

void MainMenu::update(const float dt) {
    sf::Event event;

    mouse = sf::Mouse::getPosition(game->window);
    sf::Vector2f worldPos = game->window.mapPixelToCoords(mouse);

    if(music.getStatus() != sf::Music::Playing) music.play();

    while (game->window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            game->window.close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                game->window.close();
            break;
        case sf::Event::Resized:
            view = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
            bkg[1] = sf::Vertex(sf::Vector2f(view.getSize().x, 0), color2);
            bkg[2] = sf::Vertex(sf::Vector2f(view.getSize().x, view.getSize().y), color3);
            bkg[3] = sf::Vertex(sf::Vector2f(0, view.getSize().y), color2);

            scl = view.getSize().y / 2 / sprite.getGlobalBounds().height;
            sprite.scale(sf::Vector2f(scl, scl));
            sprite.setPosition(sf::Vector2f(view.getSize().x / 10, view.getSize().y / 2 - sprite.getGlobalBounds().height / 2));

            minSize = view.getSize().y / 10.f; maxSize = minSize + 1.f;
            editorText.setCharacterSize(minSize);
            gameText.setCharacterSize(minSize);

            editorText.setPosition(sf::Vector2f(view.getSize().x / 2.25, view.getSize().y / 3));
            gameText.setPosition(sf::Vector2f(view.getSize().x / 2.25, view.getSize().y / 2));

            break;
        case sf::Event::MouseButtonReleased:
            if (event.key.code == sf::Mouse::Left) {
                if (editorText.getGlobalBounds().contains(worldPos)) {
                    editorText.setOutlineThickness(5);
                    game->pushState(new Editor(game));
                }
                if (gameText.getGlobalBounds().contains(worldPos)) {
                    gameText.setOutlineThickness(5);
                    game->pushState(new MapSelector(game));
                    music.stop();
                }
            }
            break;
        default:
            break;
        }
    }

    if (editorText.getGlobalBounds().contains(worldPos)) {
        editorText.setFillColor(sf::Color::Yellow);
        editorText.setCharacterSize(maxSize);
        editorText.setOutlineThickness(0);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            editorText.setOutlineThickness(5);
    }
    else {
        editorText.setFillColor(sf::Color::White);
        editorText.setCharacterSize(minSize);
    }
    if (gameText.getGlobalBounds().contains(worldPos)) {
        gameText.setFillColor(sf::Color::Yellow);
        gameText.setCharacterSize(maxSize);
        gameText.setOutlineThickness(0);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            gameText.setOutlineThickness(5);
    }
    else {
        gameText.setFillColor(sf::Color::White);
        gameText.setCharacterSize(minSize);
    }
}

MainMenu::MainMenu(Base* _game) {
    this->game = _game;
    initVars();
}

MainMenu::~MainMenu() {
}
