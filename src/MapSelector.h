#pragma once
#include"SFML/Graphics.hpp"
#include"State.h"
#include"Game.h"
#include"Editor.h"

#include<vector>
#include<cmath>
#include<filesystem>

class MapSelector : public State {
    struct Mapcard {
        sf::RectangleShape shape;
        sf::Text text;
        sf::Color color;
    };
    int currmap = 0, nextmap = 0;
    sf::Vector2f vel;
    sf::Vector3f colordelta;
    float R=128, G=128, B=128;
    float transition = 0.f;
    const float t_transition = .3f;
    sf::Vector2f cardsize;
    std::vector<Mapcard> maps;

    sf::Font font;

    sf::View view;
    sf::Vector2i mouse;

    sf::Music* music;
    sf::Color color1, color2, color3;
    sf::VertexArray bkg;

    void moveLeft();
    void moveRight();
    void genBkgColors();
    void clamp(float& f);
    void initVars();

public:
    virtual void render();
    virtual void update(const float dt);
    virtual void resize(int width, int height);

    MapSelector(Config* config, sf::Music* music);
    virtual ~MapSelector();
};

