#pragma once

#include <stack>
#include <SFML/Graphics.hpp>

class Runnable;

class Base {
public:
    std::stack<Runnable*> states;

    sf::RenderWindow window;

    void pushState(Runnable* state);
    void popState();
    void changeState(Runnable* state);
    Runnable* peekState();

    void GameLoop();

    Base();
    ~Base();
};
