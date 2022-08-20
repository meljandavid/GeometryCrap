#pragma once

#include <stack>
#include <memory>
#include <optional>
#include <SFML/Graphics.hpp>

#include"Config.h"

class State;

class StateManager {
public:
    std::stack<std::unique_ptr<State>> states;

    Config config;

    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);

    void GameLoop();

    StateManager();
    ~StateManager();
};
