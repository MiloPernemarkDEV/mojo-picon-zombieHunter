#pragma once

#include "Defines.h"
#include <raylib.h>
#include <vector>

#include "SauceMaker.h"
#include "Player.h"
#include "Sheep.h"
#include "FrameTool.h"
#include "Zombie.h"

struct SceneManager
{
public:
    SceneManager();

    void start();
    void update(float deltaTime);

    std::vector<i32> get_recently_killed_zombies();

    Sheep* get_sheep() { return &m_Sheep; }

private:
    void add_sauce();

    std::vector<SauceMaker*> sauces;
    Zombie m_Zombie;     
    Player m_Player;
    Sheep m_Sheep;
    FrameTool m_FrameTool;
    std::vector<i32> m_recentlyKilledZombies;
};