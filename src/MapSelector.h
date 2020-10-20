#pragma once
#include"SFML/Graphics.hpp"
#include"Runnable.h"
#include"Game.h"

#include<fstream>
#include<vector>

struct lButton {
    std::string id;
    sf::FloatRect box;
    sf::Color bkg=sf::Color(255, 255, 255, 64), onPress=sf::Color::Yellow,
        textCol=sf::Color::Black, hover= sf::Color(0, 0, 0, 64), color;
    float outline = 0;
};

class MapSelector : public Runnable {
    sf::View view;
    sf::Vector2i mouse;

    sf::RectangleShape shape;
    sf::Font font;
    sf::Text text;
    lButton b_mymap;

    sf::Color color1, color2, color3;
    sf::VertexArray bkg;

    std::ifstream in;
    std::vector<lButton> buttons;

    bool rip = false;
    void initVars();

public:
    virtual void render();
    virtual void update(const float dt);

    MapSelector(Base* game);
    virtual ~MapSelector();
};

