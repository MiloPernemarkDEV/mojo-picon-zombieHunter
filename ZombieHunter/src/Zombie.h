#pragma once

#include "SauceMaker.h"
#include "Defines.h"
#include <vector>

struct Player;

const i32 START_ZOMBIE_COUNT{ 10 };
const i32 MAX_ZOMBIE_COUNT{ 1000 };

const i32 WALKER_ZOMBIE_HEALTH{ 30 };
const i32 RUNNER_ZOMBIE_HEALTH{ 20 };

enum class ZombieType
{
    WALKER,
    RUNNER
};

struct Zombie : public SauceMaker
{
public:
    Zombie();
    ~Zombie();

    void update(float deltaTime) override;
    void draw_sprite() override;
    void start() override;

    void move_zombies(float deltaTime);
    void apply_primary_attack(Vector2 playerPos);
    void apply_secondary_attack(Vector2 playerPos);
    void start_wave(i32 waveIndex, Vector2 center);
    void attack_player(Player& player, float deltaTime);
    void damage_zombie(i32 index, i32 dmg);

    // Implicitly inline
    i32 get_zombie_count() const;
    Vector2 get_zombie_position(i32 index) const { return m_zombiePositions[index]; };
    i32 get_zombie_health(i32 index) const { return m_zombieHealth[index]; };

    i32 get_alive_count() const;
    std::vector<i32> get_killed_since_last_update(); 
    Vector2 m_playerPos;
private:
    std::vector<Vector2> m_zombiePositions;
    std::vector<ZombieType> m_zombieTypes;
    std::vector<i32> m_zombieHealth;

    i32 m_walkerSpeed;
    i32 m_runnerSpeed;

    i32 m_currentZombieCount;

    float m_zombieRadius;
    float m_attackRange;

    float m_attackTimer;
    float m_attackCooldown;

    std::vector<i32> m_recentlyKilled; 
};