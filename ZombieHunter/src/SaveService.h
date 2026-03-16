#pragma once
#include <vector>
#include "Defines.h" 


class HUD;
class Sheep;
class GameManager;

class SaveService {
public:
    SaveService(HUD* hud, Sheep* sheep);

    void load_data();
    void save_data();
    void clear_data();

private:
    HUD* m_hud;
    Sheep* m_sheep;
    i32 m_killedZombies;
    bool m_sheepHasSpawned;
};