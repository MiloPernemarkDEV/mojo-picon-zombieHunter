#pragma once 

#include <raylib.h>

class SauceMaker {
public:
	SauceMaker() = default;
	virtual~SauceMaker() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void drawSprite() = 0;
	virtual void start() = 0;
	Transform position;
	Vector2 texture;
private:

};