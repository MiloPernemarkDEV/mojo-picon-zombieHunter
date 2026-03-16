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

	bool consume_spawn_wave_request();

	i32 set_current_wave(i32 wave);
	i32 get_current_wave() const { return m_currentWave; }
	i32 set_item_count(i32 count);
	i32 get_item_count() const {return m_itemCount; };
	void add_item_to_count(i32 count);
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
	i32 m_itemCount{0};
};

