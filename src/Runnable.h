#pragma once
#include "Base.h"

class Runnable {
public:
	Base* game;
	virtual void update(const float dt) = 0;
	virtual void render() = 0;

	virtual ~Runnable() { }
};