#include "SaveService.h"
#include <fstream>

SaveService::SaveService(HUD& hud, Sheep* sheep)
    : m_hud(&hud),
    m_sheep(sheep),
    m_loadedWave(0),
    m_currentWave(0),
    m_killedZombies(0)

{
}

void SaveService::load_data()
{
    std::ifstream stream("save.bin", std::ios::binary);

    if (stream.is_open()) {
        i32 wave, kills;
        bool spawned;

        stream.read(reinterpret_cast<char*>(&wave), sizeof(wave));
        stream.read(reinterpret_cast<char*>(&kills), sizeof(kills));
        stream.read(reinterpret_cast<char*>(&spawned), sizeof(spawned));

        GameManager::Instance().setCurrentWave(wave);
        m_hud->setZombiesKilled(kills);
        m_sheep->setHasSpawned(spawned);

        size_t count = 0;
        if (stream.read(reinterpret_cast<char*>(&count), sizeof(count))) {
            std::vector<Vector2> loadedPos(count); 

            stream.read(reinterpret_cast<char*>(loadedPos.data()), count * sizeof(Vector2));

            m_sheep->setLoadedPositions(loadedPos);
        }

        stream.close();
    }
}

void SaveService::save_data()
{
    std::ofstream stream("save.bin", std::ios::binary);

    if (stream.is_open()) {
        i32 wave = GameManager::Instance().getCurrentWave();
        i32 kills = m_hud->getZombieKills();
        bool spawned = m_sheep->getHasSpawned();

        stream.write(reinterpret_cast<const char*>(&wave), sizeof(wave));
        stream.write(reinterpret_cast<const char*>(&kills), sizeof(kills));
        stream.write(reinterpret_cast<const char*>(&spawned), sizeof(spawned));

        const auto& positions = m_sheep->getPositions();
        size_t count = positions.size();

        stream.write(reinterpret_cast<const char*>(&count), sizeof(count));

        if (count > 0) {
            stream.write(reinterpret_cast<const char*>(positions.data()), count * sizeof(Vector2));
        }

        stream.close();
    }
}