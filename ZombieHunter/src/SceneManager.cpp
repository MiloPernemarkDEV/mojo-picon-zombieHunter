#include "SceneManager.h"
#include "GameManager.h"

SceneManager::SceneManager()
    : m_Zombie(),
      m_Player(m_Zombie)
{
}

void SceneManager::start() 
{
    add_sauce();

    for (i32 i = 0; i < sauces.size(); i++) 
        sauces[i]->start();

    GameManager::Instance().start();

    m_Zombie.start_wave(1, m_Player.get_player_position());
}

void SceneManager::update(float deltaTime) 
{
    m_Sheep.m_playerPos = m_Player.get_player_position();
    m_Zombie.m_playerPos = m_Player.get_player_position();

    for (i32 i = 0; i < sauces.size(); i++) 
        sauces[i]->update(deltaTime);

    if (m_Player.is_primary_attack_triggered())
        m_Zombie.apply_primary_attack(m_Player.get_player_position());

    if (m_Player.is_secondary_attack_triggered())
        m_Zombie.apply_secondary_attack(m_Player.get_player_position());

    m_Zombie.attack_player(m_Player, deltaTime);

    i32 alive = m_Zombie.get_alive_count();
    GameManager::Instance().update(deltaTime, alive);
    if (GameManager::Instance().consume_spawn_wave_request())
        m_Zombie.start_wave(GameManager::Instance().get_current_wave(), m_Player.get_player_position());

    m_recentlyKilledZombies = m_Zombie.get_killed_since_last_update();
}

std::vector<i32> SceneManager::get_recently_killed_zombies()
{
    return m_recentlyKilledZombies;
}

void SceneManager::add_sauce()
{
    sauces.push_back(&m_FrameTool);
    sauces.push_back(&m_Player);
    sauces.push_back(&m_Sheep);
    sauces.push_back(&m_Zombie);
}