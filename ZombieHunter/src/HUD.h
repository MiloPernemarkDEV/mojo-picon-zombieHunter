#pragma once

#include "Defines.h"
#include "GameManager.h"
#include <raylib.h>

class HUD
{
public:
    HUD();

    void add_zombie_kill();
    void update();
    void draw();
    
    void set_zombies_killed(int amount);
    i32 get_zombies_killed() const { return m_zombiesKilled; }
    i32 get_passive_bonus() const { return m_passiveBonus; }

private:
    i32 m_zombiesKilled;
    i32 m_passiveBonus; 

    const i32 m_killsPerUpgrade = 10;
};