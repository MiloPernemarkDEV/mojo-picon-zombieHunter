#include "Player.h"

#include <iostream>
#include "SpriteFilePaths.h"

Player::Player()
	: m_spriteSheet(NULL),
	position(720, 400),
	speed(0),
	walkSpeed(2.7f),
	sprintSpeed(8.0f),
	frameCount(8),
	frameWidth(0),
	frameHeight(0),
	frameIndex(0)
{
}

Player::~Player()
{
}

void Player::update(float deltaTime) {
	drawSprite();
	moveController();
}

void Player::drawSprite()
{
	// Part of the spreadsheet to cut out
	Rectangle sourceRect = { (float)frameIndex * frameWidth, 0, (float)frameWidth, (float)frameHeight };

	float scale = 3.0f;
	Rectangle destRect = { position.x, position.y, frameWidth * scale, frameHeight * scale };

	Vector2 origin = { 200 , 400 };

	DrawTexturePro(m_spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);
}

void Player::moveController()
{
	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		speed = sprintSpeed;
	}
	else {
		speed = walkSpeed;
	}

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) 
		position.y -= speed;

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) 
		position.x -= speed;

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) 
		position.y += speed;

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) 
		position.x += speed;
}

void Player::start()
{
	m_spriteSheet = {LoadTexture(SFP::PLAYER_PNG)};
	SetTextureFilter(m_spriteSheet, TEXTURE_FILTER_POINT);

	frameWidth = m_spriteSheet.width / frameCount;
	frameHeight = m_spriteSheet.height;
}

Vector2 Player::getPlayerPosition()
{
	return position;
}


