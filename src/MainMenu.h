#pragma once

#include"SFML/Graphics.hpp"
#include"State.h"
#include"Game.h"
#include"Editor.h"
#include"MapSelector.h"
#include"EditorLauncher.h"

#include<iostream>

class MainMenu : public State {
    sf::View view;
    sf::Vector2i mouse;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::Shader shader;
    sf::VertexArray sharr;
    float elapsed;

    sf::Font font;
    sf::Text editorText, gameText;
    sf::Color cdef, chover;
    float es, gs;
    float minSize, maxSize;

    sf::Color color1, color2, color3;
    sf::VertexArray bkg;
    float scl;

    sf::RectangleShape volshape, dragshape;

    sf::Music music;

    void initVars();

public:
    virtual void render();
    virtual void update(const float dt);
    virtual void resize(int width, int height);

    MainMenu(Config* config);
    virtual ~MainMenu();
};
