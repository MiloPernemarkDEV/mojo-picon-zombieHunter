#include "GameManager.h"

GameManager& GameManager::Instance()
{
	static GameManager instance;
	return instance;
}

void GameManager::start()
{
	m_state = GameState::PLAYING;
	if (m_currentWave == 0){ m_currentWave = 0; }
	m_waveTimer = 0.0f;
	m_waitingForNextWave = false;
	m_spawnNextWave = true;
}

void GameManager::update(float deltaTime, i32 aliveEnemies)
{
	if (m_state != GameState::PLAYING)
	{
		return;
	}

	
	if (aliveEnemies > 0)
	{
		return;
	}

	if (!m_waitingForNextWave)
	{
		m_waitingForNextWave = true;
		m_waveTimer = 0.0f;
	}

	m_waveTimer += deltaTime;
	if (m_waveTimer >= m_timeBetweenWaves)
	{
		m_waitingForNextWave = false;
		m_waveTimer = 0.0f;
		m_currentWave++;
		m_spawnNextWave = true;
	}
}

bool GameManager::consume_spawn_wave_request()
{
	if (!m_spawnNextWave)
	{
		return false;
	}

	m_spawnNextWave = false;

	return true;
}

i32 GameManager::set_current_wave(i32 wave)
{
	return m_currentWave = wave;
}

i32 GameManager::set_item_count(i32 count)
{
	return m_itemCount = count;
}

void GameManager::add_item_to_count(i32 count)
{
	if (count <= 0) { return; }
	m_itemCount += count;
}

