#include "HUD.h"
#include "SaveService.h"
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

void HUD::update(float deltaTime)
{
    static bool tabWasPressed = false;

    if (IsKeyDown(KEY_TAB))
    {
        if (!tabWasPressed)
        {
            m_showSettings = !m_showSettings;
            tabWasPressed = true;
        }
    }
    else
    {
        tabWasPressed = false;
    }
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

    if (m_showSettings)
        draw_settings();

}

void HUD::draw_settings()
{
    const float menuWidth = 300;
    const float menuHeight = 250;
    const float padding = 20;
    const float screenRightX = GetScreenWidth() - menuWidth - padding;
    const float screenTopY = padding;

    DrawRectangle(screenRightX, screenTopY, menuWidth, menuHeight, Fade(GRAY, 0.8f));

    int y = screenTopY + 30;
    int lineSpacing = 30;

    DrawText("Controls", screenRightX + 80, y, 20, WHITE);
    y += lineSpacing;

    DrawText("W/A/S/D or Arrow Keys - Move", screenRightX + 20, y, 18, WHITE);
    y += lineSpacing;

    DrawText("Left Shift - Sprint", screenRightX + 20, y, 18, WHITE);
    y += lineSpacing;

    DrawText("Left Mouse Click - Shoot", screenRightX + 20, y, 18, WHITE);
    y += lineSpacing;

    DrawText("TAB - Toggle Settings", screenRightX + 20, y, 18, WHITE);
    y += lineSpacing + 10;
}

void HUD::set_zombies_killed(int amount)
{
    m_zombiesKilled = amount;
    m_passiveBonus = m_zombiesKilled / m_killsPerUpgrade;
}
