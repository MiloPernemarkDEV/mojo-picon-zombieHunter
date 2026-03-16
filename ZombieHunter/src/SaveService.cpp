#include "SaveService.h"
#include <fstream>
#include "HUD.h"
#include "GameManager.h"
#include "Sheep.h"

SaveService::SaveService(HUD* hud, Sheep* sheep)
    : m_hud(hud),
    m_sheep(sheep),
    m_killedZombies(0)
{
}

    void SaveService::load_data()
    {
        std::ifstream stream("save.bin", std::ios::binary);

        if (!stream.is_open())
            return;

        if (stream.is_open()) {
            i32 wave, kills;
            bool spawned;

            if (!stream.read(reinterpret_cast<char*>(&wave), sizeof(wave))) return;
            if (!stream.read(reinterpret_cast<char*>(&kills), sizeof(kills))) return;
            if (!stream.read(reinterpret_cast<char*>(&spawned), sizeof(spawned))) return;

            // more

            GameManager::Instance().set_current_wave(wave);
            m_hud->set_zombies_killed(kills);
            m_sheep->set_has_spawned(spawned);

            size_t count = 0;
            if (stream.read(reinterpret_cast<char*>(&count), sizeof(count))) {
                std::vector<Vector2> loadedPos(count); 

                stream.read(reinterpret_cast<char*>(loadedPos.data()), count * sizeof(Vector2));

                m_sheep->set_loaded_positions(loadedPos);
            }

            stream.close();
        }
    }

void SaveService::save_data()
{
    std::ofstream stream("save.bin", std::ios::binary);

    if (stream.is_open()) {
        i32 wave = GameManager::Instance().get_current_wave();
        i32 kills = m_hud->get_zombies_killed();
        bool spawned = m_sheep->get_has_spawned();

        stream.write(reinterpret_cast<const char*>(&wave), sizeof(wave));
        stream.write(reinterpret_cast<const char*>(&kills), sizeof(kills));
        stream.write(reinterpret_cast<const char*>(&spawned), sizeof(spawned));

        // more

        const auto& positions = m_sheep->get_positions();
        size_t count = positions.size();

        stream.write(reinterpret_cast<const char*>(&count), sizeof(count));

        if (count > 0) {
            stream.write(reinterpret_cast<const char*>(positions.data()), count * sizeof(Vector2));
        }

        stream.close();
    }
}

void SaveService::clear_data()
{
    std::ofstream("save.bin", std::ios::binary | std::ios::trunc);
}
