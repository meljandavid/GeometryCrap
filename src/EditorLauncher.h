#pragma once
#include "State.h"
#include "Button.h"
#include "Editor.h"
#include <SFML/Audio/Music.hpp>

class EditorLauncher : public State {
    sf::View view;
    sf::VertexArray bkg;
    sf::Color color1, color2, color3;

    sf::Font font;
    Button b_create;

    std::vector<Button> buttons;
    sf::Text text;
    sf::RectangleShape shape;

    void initVars();

public:
    virtual void render();
    virtual void update(const float dt);
    virtual void resize(int width, int height);

    EditorLauncher(Config* config);
    virtual ~EditorLauncher();
};

