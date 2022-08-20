#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "StateManager.h"
#include "State.h"

void StateManager::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void StateManager::popState() {
    states.pop();

    if (!states.empty()) {
        states.top()->resize(config.window.getSize().x, config.window.getSize().y);
    }
}

void StateManager::changeState(std::unique_ptr<State> state) {
    if (!states.empty()) popState();
    pushState(std::move(state));
}

void StateManager::GameLoop() {
    sf::Clock clock;
    float prevdt = 1.f;
    float dt = 1.f;
    while (config.window.isOpen()) {
        prevdt = dt;
        dt = clock.restart().asSeconds();

        if (states.empty()) continue;
        
        // 200 IQ solution to sync the game with loadtimes
        states.top()->update(std::min(dt, prevdt));
        states.top()->render();
        config.window.display();

        auto ps = states.top()->pendingState();
        if (ps != nullptr) {
            pushState(std::move(ps));
        }

        else if (states.top()->shouldExit()) {
            popState();
        }
    }
}

StateManager::StateManager() { }

StateManager::~StateManager() {
    while (!states.empty()) popState();
}
