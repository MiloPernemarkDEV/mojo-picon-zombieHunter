#include "HUD.h"
#include <string>
#include <sstream>

HUD::HUD()
    : m_zombiesKilled(0),
    m_passiveBonus(0)
{
}

void HUD::add_zombie_kill()
{
    m_zombiesKilled++;

    // Every m_killsPerUpgrade zombies, increase passive bonus
    if (m_zombiesKilled % m_killsPerUpgrade == 0)
    {
        m_passiveBonus++;
    }
}

void HUD::update()
{
   
}

void HUD::draw()
{
    std::stringstream waveText;
    waveText << "Wave: " << GameManager::Instance().get_current_wave();
    DrawText(waveText.str().c_str(), 20, 20, 20, WHITE);

    std::stringstream killsText;
    killsText << "Zombies killed: " << m_zombiesKilled;
    DrawText(killsText.str().c_str(), 20, 50, 20, WHITE);

    std::stringstream bonusText;
    bonusText << "Passive bonus: +" << m_passiveBonus;
    DrawText(bonusText.str().c_str(), 20, 80, 20, YELLOW);
}

void HUD::set_zombies_killed(int amount)
{
    m_zombiesKilled = amount;
    m_passiveBonus = m_zombiesKilled / m_killsPerUpgrade;
}
