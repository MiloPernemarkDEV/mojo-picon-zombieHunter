#pragma once

#include "SauceMaker.h"
#include <stdint.h>
#include "Defines.h"

struct Player : public SauceMaker
{
public:
	Player();
	~Player();
	void update(float deltaTime) override;
	void drawSprite() override;
	void moveController();
	void start() override;
	
	Vector2 getPlayerPosition();

private:
	Texture2D m_spriteSheet;
	i32 frameCount;
	i32 frameWidth;
	i32 frameIndex;
	i32 frameHeight;
	float speed;
	float walkSpeed;
	float sprintSpeed;
	Vector2 position;
};

