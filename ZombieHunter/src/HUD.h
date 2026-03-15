#pragma once

#include "Defines.h"
#include "GameManager.h"
#include <raylib.h>

class HUD
{
public:
    HUD();

    void addZombieKill();
    void update();
    void draw();
    
    void setZombiesKilled(int amount);
    i32 getZombieKills() const { return m_zombiesKilled; }
    i32 getPassiveBonus() const { return m_passiveBonus; }

private:
    i32 m_zombiesKilled;
    i32 m_passiveBonus; 

    const i32 m_killsPerUpgrade = 10;
};