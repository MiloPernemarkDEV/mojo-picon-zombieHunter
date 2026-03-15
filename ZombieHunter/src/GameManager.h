#pragma once

#include "Defines.h"

enum class GameState
{
	MENU,
	PLAYING,
	GAME_OVER
};

class GameManager
{
public:
	static GameManager& Instance();

	void start();
	void update(float deltaTime, i32 aliveEnemies);

	bool consumeSpawnWaveRequest();

	i32 setCurrentWave(i32 wave);
	i32 getCurrentWave() const { return m_currentWave; }
	GameState getGameState() const { return m_state; }

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	GameState m_state{ GameState::PLAYING };
	i32 m_currentWave{ 0 };
	float m_timeBetweenWaves{ 3.0f };
	float m_waveTimer{ 0.0f };
	bool m_waitingForNextWave{ false };
	bool m_spawnNextWave{ false };
};

