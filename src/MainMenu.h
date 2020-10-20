#pragma once

#include"SFML/Graphics.hpp"
#include"Runnable.h"
#include"Game.h"
#include"Editor.h"
#include"MapSelector.h"

class MainMenu : public Runnable {
    sf::View view;
    sf::Vector2i mouse;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::Font font;
    sf::Text editorText, gameText;
    float minSize, maxSize;

    sf::Color color1, color2, color3;
    sf::VertexArray bkg;
    float scl;

    sf::Music music;

    void initVars();

public:
    virtual void render();
    virtual void update(const float dt);

    MainMenu(Base* game);
    virtual ~MainMenu();
};
