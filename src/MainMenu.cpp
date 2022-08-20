#include"SFML/Graphics.hpp"

#include "MainMenu.h"
#include "MapSelector.h"
#include "State.h"

#include"Log.h"

void MainMenu::render() {
    config->window.setView(view);

    config->window.clear();

    config->window.draw(bkg);
    config->window.draw(sharr, &shader);
    config->window.draw(sprite);
    config->window.draw(editorText);
    config->window.draw(gameText);
    config->window.draw(volshape);
    config->window.draw(dragshape);
}

void MainMenu::initVars() {
    view = config->window.getDefaultView();

    minSize = config->window.getSize().y / 10.f;
    maxSize = config->window.getSize().y / 9.f;

    font.loadFromFile("Assets/Fonts/Dragz.otf");

    chover = sf::Color::Yellow;
    cdef = sf::Color::White;

    es = gs = minSize;

    editorText.setFont(font);
    editorText.setString("{EDITOR}");
    editorText.setPosition(sf::Vector2f(config->window.getSize().x / 2.25f, config->window.getSize().y / 3.f));
    editorText.setCharacterSize(minSize);
    editorText.setFillColor(cdef);
    gameText.setFont(font);
    gameText.setString("{PLAY}");
    gameText.setPosition(sf::Vector2f(config->window.getSize().x / 2.25f, config->window.getSize().y / 2.f));
    gameText.setCharacterSize(minSize);
    gameText.setFillColor(cdef);

    if (!music.openFromFile("Assets/Music/keppelskies_fiberman.ogg")) {
        std::cout << "rip music :( \n";
    }
    else {
        music.setPlayingOffset(sf::seconds(0));
        music.setVolume(0.f);
        music.play();
        music.setLoop(true);
    }

    color1 = sf::Color::White;
    color2 = sf::Color(96, 96, 96);
    color3 = sf::Color::Black;

    bkg.setPrimitiveType(sf::Quads);
    bkg.resize(4);
    bkg[0] = sf::Vertex(sf::Vector2f(0, 0), color1);
    bkg[1] = sf::Vertex(sf::Vector2f(config->window.getSize().x, 0), color2);
    bkg[2] = sf::Vertex(sf::Vector2f(config->window.getSize().x, config->window.getSize().y), color3);
    bkg[3] = sf::Vertex(sf::Vector2f(0, config->window.getSize().y), color2);

    texture.loadFromFile("Assets/Textures/logoM.png");
    texture.setSmooth(true);
    sprite.setTexture(texture);
    scl = config->window.getSize().y / 2.f / sprite.getGlobalBounds().height;
    sprite.setScale(sf::Vector2f(scl, scl));
    sprite.setPosition(sf::Vector2f(config->window.getSize().x / 10.f, config->window.getSize().y / 2.f - sprite.getGlobalBounds().height / 2));

    if (!shader.isAvailable()) {
        std::ofstream eo("shaderna.txt");
        eo << "Shaders not available :(\n";
        eo.close();
    }
    if (!shader.loadFromFile("Assets/Textures/wave.frag", sf::Shader::Fragment)) {
        std::ofstream eo("shadernf.txt");
        eo << "Shader not found :(\n";
        eo.close();
    }
    sharr.setPrimitiveType(sf::Quads);
    sharr.resize(4);
    sharr[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::Transparent);
    sharr[1] = sf::Vertex(sf::Vector2f(config->window.getSize().x, 0), sf::Color::Transparent);
    sharr[2] = sf::Vertex(sf::Vector2f(config->window.getSize().x, config->window.getSize().y), sf::Color::Transparent);
    sharr[3] = sf::Vertex(sf::Vector2f(0, config->window.getSize().y), sf::Color::Transparent);
    sf::Vector2f wndSize = sf::Vector2f(config->window.getSize().x, config->window.getSize().y);
    shader.setUniform("resolution", wndSize);

    auto volumebarbkg = sf::FloatRect(config->window.getSize().x - 200.f, 10.f,
        config->window.getSize().x / 8.f, 25.f);
    volshape.setPosition(sf::Vector2f(volumebarbkg.left, volumebarbkg.top));
    volshape.setSize(sf::Vector2f(volumebarbkg.width, volumebarbkg.height));
    volshape.setFillColor(sf::Color::Transparent);
    volshape.setOutlineColor(sf::Color::Black);
    volshape.setOutlineThickness(2.f);
    dragshape.setFillColor(sf::Color::Red);
    dragshape.setSize(sf::Vector2f(10.f, volumebarbkg.height));
    dragshape.setPosition(sf::Vector2f(
        volumebarbkg.left + config->volume * volumebarbkg.width,
        volumebarbkg.top)
    );
    
}

void MainMenu::update(const float dt) {
    elapsed += dt;
    shader.setUniform("time", elapsed);

    mouse = sf::Mouse::getPosition(config->window);
    sf::Vector2f worldPos = config->window.mapPixelToCoords(mouse);

    if(music.getStatus() != sf::Music::Playing) music.play();
    if (music.getVolume() < config->volume*100.f) {
        music.setVolume( std::min( music.getVolume() + dt*50.f, config->volume*100.f) );
    }

    sf::Event event;
    while (config->window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            config->window.close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                config->window.close();
            break;
        case sf::Event::Resized:
            resize(event.size.width, event.size.height);

            break;
        case sf::Event::MouseButtonReleased:
            if (event.key.code == sf::Mouse::Left) {
                if (editorText.getGlobalBounds().contains(worldPos)) {
                    editorText.setOutlineThickness(5);
                    pendingstate = std::make_unique<EditorLauncher>( config );
                }
                if (gameText.getGlobalBounds().contains(worldPos)) {
                    gameText.setOutlineThickness(5);
                    pendingstate = std::make_unique<MapSelector>( config, &music );
                }
            }
            break;
        default:
            break;
        }
    }

    if (editorText.getGlobalBounds().contains(worldPos)) {
        editorText.setFillColor(chover);
        es = std::min(es + 50 * dt, maxSize);
        editorText.setCharacterSize(unsigned int(es));
        editorText.setOutlineThickness(0);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            editorText.setOutlineThickness(5);
    }
    else {
        editorText.setFillColor(cdef); 
        es = std::max(es - 50 * dt, minSize);
        editorText.setCharacterSize(unsigned int(es));
    }
    if (gameText.getGlobalBounds().contains(worldPos)) {
        gameText.setFillColor(chover);
        gs = std::min(gs + 50*dt, maxSize);
        gameText.setCharacterSize(unsigned int(gs));
        gameText.setOutlineThickness(0);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            gameText.setOutlineThickness(5);
    }
    else {
        gameText.setFillColor(cdef);
        gs = std::max(gs - 50 * dt, minSize);
        gameText.setCharacterSize(unsigned int(gs));
    }

    if (sf::FloatRect(volshape.getPosition().x+4.f, volshape.getPosition().y,
        volshape.getSize().x-4.f, volshape.getSize().y).contains(worldPos) &&
        volshape.getGlobalBounds().contains(worldPos) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        dragshape.setPosition(sf::Vector2f(
            worldPos.x - dragshape.getSize().x / 2.f,
            dragshape.getPosition().y)
        );
        config->volume = (dragshape.getPosition().x - volshape.getPosition().x)
            / volshape.getSize().x;
        if (config->volume < 0.f) config->volume = 0.001f;
        music.setVolume(config->volume * 100);
    }
}

void MainMenu::resize(int width, int height) {
    view = sf::View(sf::FloatRect(0, 0, width, height));

    bkg[1] = sf::Vertex(sf::Vector2f(width, 0), color2);
    bkg[2] = sf::Vertex(sf::Vector2f(width, height), color3);
    bkg[3] = sf::Vertex(sf::Vector2f(0, height), color2);

    scl = height / 2.f / sprite.getGlobalBounds().height;
    sprite.scale(sf::Vector2f(scl, scl));
    sprite.setPosition(sf::Vector2f(width / 10.f, height / 2.f - sprite.getGlobalBounds().height / 2.f));

    minSize = height / 10.f;
    maxSize = height / 9.f;
    editorText.setCharacterSize(minSize);
    gameText.setCharacterSize(minSize);

    editorText.setPosition(sf::Vector2f(width / 2.25f, height / 3.f));
    gameText.setPosition(sf::Vector2f(width / 2.25f, height / 2.f));

    sharr[1] = sf::Vertex(sf::Vector2f(width, 0), sf::Color::Transparent);
    sharr[2] = sf::Vertex(sf::Vector2f(width, height), sf::Color::Transparent);
    sharr[3] = sf::Vertex(sf::Vector2f(0, height), sf::Color::Transparent);
    sf::Vector2f wndSize = sf::Vector2f(float(width), float(height));
    shader.setUniform("resolution", wndSize);

    volshape.setSize({ config->window.getSize().x / 8.f, 25.f });
    volshape.setPosition({
        width - volshape.getSize().x - 20.f, 10.f });
    
    dragshape.setPosition(sf::Vector2f(
        volshape.getPosition().x + config->volume * volshape.getSize().x,
        volshape.getPosition().y)
    );
}

MainMenu::MainMenu(Config* config) {
    this->config = config;
    initVars();
}

MainMenu::~MainMenu() {
}
