#pragma once
#include <vector>
#include "Defines.h"
#include <raylib.h>
#include "GameManager.h"

class SaveService; 

class HUD
{
public:
    HUD();

    void update(float deltaTime);
    void draw();
    void add_zombie_kill();
    void set_zombies_killed(int amount);

    void setSaveService(SaveService* saveService) { m_saveService = saveService; }
    int get_zombies_killed() const { return m_zombiesKilled; }

private:
    void draw_settings();

    int m_zombiesKilled;
    int m_passiveBonus;
    const int m_killsPerUpgrade = 5;
    bool m_showSettings = false;

    SaveService* m_saveService = nullptr;  
};