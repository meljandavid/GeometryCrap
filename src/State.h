#pragma once
#include "StateManager.h"

#include"Log.h"

class State {
protected:
	bool shouldClose = false;
	std::unique_ptr<State> pendingstate;
	Config* config = nullptr;
public:
	virtual void update(const float dt) = 0;
	virtual void render() = 0;
	virtual void resize(int width, int height) = 0;

	std::unique_ptr<State> pendingState() {
		return std::move(pendingstate);
	}
	bool shouldExit() { return shouldClose; }

	virtual ~State() { }
};