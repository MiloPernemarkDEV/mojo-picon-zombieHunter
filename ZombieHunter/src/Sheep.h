/////////////
// Sheep.h //
/////////////

// Using SoA (struct of arrays) so each data type is tightly packed in memory.
// This improves cache efficiency as they are used a lot per frame.

#pragma once

#include "SauceMaker.h"
#include "Defines.h"
#include <vector>
#include "Player.h"

#include "MojoPiconMath.h"


const i32 START_SHEEP_COUNT{30};
const i32 MAX_SHEEP_COUNT{30};
const float MAX_DISTANCE_TO_PLAYER{300};

enum class SheepStates {
	IDLE,
	GRAZING,
	FLEEING
};

struct Sheep : public SauceMaker {
public:
	Sheep();
	~Sheep();

	void update(float deltaTime) override;
	void draw_sprite() override;
	void move_sheep(float deltaTime);
	void start() override;
	Vector2 flee_direction(i32 i);
	SheepStates state_machine(i32 index, float deltaTime);

	Vector2 m_playerPos;
	bool get_has_spawned() const {return m_hasSpawned; };
	void set_has_spawned(bool hasSpawned);
	void set_loaded_positions(const std::vector<Vector2>& positions);
	const std::vector<Vector2>& get_positions() const { return m_sheepPositions; };
private:
	std::vector<Vector2> m_sheepPositions;
	std::vector<Vector2> m_sheepDirections;
	std::vector<SheepStates> m_sheepStates;
	std::vector<float> m_sheepStateTimers;

	Texture2D m_texture; // 20 bytes
	i32 m_fleeSpeed;
	i32 m_wanderSpeed;
	i32 m_currentSheepCount;
	bool m_hasSpawned;
};