#include "SceneManager.h"
#include "GameManager.h"

SceneManager::SceneManager()
    : m_Zombie(),
      m_Player(m_Zombie)
{
}

void SceneManager::start() 
{
    addSauce();

    for (i32 i = 0; i < sauces.size(); i++) 
        sauces[i]->start();

    GameManager::Instance().start();

    m_Zombie.startWave(1, m_Player.getPlayerPosition());
}

void SceneManager::update(float deltaTime) 
{
    m_Sheep.m_playerPos = m_Player.getPlayerPosition();
    m_Zombie.m_playerPos = m_Player.getPlayerPosition();

    for (i32 i = 0; i < sauces.size(); i++) 
        sauces[i]->update(deltaTime);

    if (m_Player.isPrimaryAttackTriggered())
        m_Zombie.applyPrimaryAttack(m_Player.getPlayerPosition());

    if (m_Player.isSecondaryAttackTriggered())
        m_Zombie.applySecondaryAttack(m_Player.getPlayerPosition());

    m_Zombie.attackPlayer(m_Player, deltaTime);

    i32 alive = m_Zombie.getAliveCount();
    GameManager::Instance().update(deltaTime, alive);
    if (GameManager::Instance().consumeSpawnWaveRequest())
        m_Zombie.startWave(GameManager::Instance().getCurrentWave(), m_Player.getPlayerPosition());

    m_recentlyKilledZombies = m_Zombie.getKilledSinceLastUpdate();
}

std::vector<i32> SceneManager::getRecentlyKilledZombies()
{
    return m_recentlyKilledZombies;
}

void SceneManager::addSauce()
{
    sauces.push_back(&m_FrameTool);
    sauces.push_back(&m_Player);
    sauces.push_back(&m_Sheep);
    sauces.push_back(&m_Zombie);
}