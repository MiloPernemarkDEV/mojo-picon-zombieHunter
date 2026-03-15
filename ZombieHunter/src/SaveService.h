#pragma once
#include <iostream>
#include <vector>
#include "GameManager.h"
#include "HUD.h"
#include "Sheep.h"
#include "Defines.h" 

class SaveService {
public:
    SaveService(HUD& hud, Sheep* sheep);

    void load_data();
    void save_data();

private:
    HUD* m_hud; 
    Sheep* m_sheep;

    i32 m_loadedWave;
    i32 m_currentWave;
    i32 m_killedZombies;
    bool m_sheepHasSpawned;
};