// We want to spawn START_SHEEP_COUNT amount of sheep at the start of the game just like the ai 
// They are going to be hidden and we simulate them spawning dynamically 

#pragma once

#include "SauceMaker.h"
#include "Defines.h"
#include <vector>
#include "Player.h"

#include "MojoPiconMath.h"


const i32 START_SHEEP_COUNT{20};
const i32 MAX_SHEEP_COUNT{20};
const float MAX_DISTANCE_TO_PLAYER{300};

static enum class SheepStates {
	IDLE,
	GRAZING,
	FLEEING
};

struct Sheep : public SauceMaker {
public:
	Sheep();
	~Sheep();

	void update(float deltaTime) override;
	void drawSprite() override;
	void moveSheep();
	void start() override;
	Vector2 m_playerPos;
	SheepStates stateMachine(i32 index, float deltaTime);
	Vector2 fleeDirection(i32 i);
private:
	std::vector<Vector2> m_sheepPositions;
	std::vector<Vector2> m_sheepDirections;
	std::vector<SheepStates> m_sheepStates;
	std::vector<float> m_sheepStateTimers;

	Texture2D m_texture;
	MojoPiconMath m_math;
	i32 m_fleeSpeed;
	i32 m_wanderSpeed;
	i32 m_currentSheepCount;
};