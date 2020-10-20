#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Base.h"
#include "Runnable.h"

void Base::pushState(Runnable* state) {
    this->states.push(state);

    return;
}

void Base::popState() {
    delete this->states.top();
    this->states.pop();

    return;
}

void Base::changeState(Runnable* state) {
    if (!this->states.empty())
        popState();
    pushState(state);

    return;
}

Runnable* Base::peekState() {
    if (this->states.empty()) return nullptr;
    return this->states.top();
}

void Base::GameLoop() {
    sf::Clock clock;

    while (this->window.isOpen()) {
        float dt = clock.restart().asSeconds();

        if (peekState() == nullptr) continue;

        peekState()->update(dt);
        this->window.clear(sf::Color::Black);
        peekState()->render();
        window.display();
    }
}

Base::Base() {
    this->window.create(sf::VideoMode(1280, 720), "GeometryCrap");
}

Base::~Base() {
    while (!this->states.empty()) popState();
}
