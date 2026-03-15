#pragma once

#include "SauceMaker.h"
#include "Defines.h"
#include <vector>

struct Player;

const i32 START_ZOMBIE_COUNT{ 10 };
const i32 MAX_ZOMBIE_COUNT{ 30 };

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
    void drawSprite() override;
    void start() override;

    void moveZombies(float deltaTime);

    void applyPrimaryAttack(Vector2 playerPos);
    void applySecondaryAttack(Vector2 playerPos);

    void startWave(i32 waveIndex, Vector2 center);

    void attackPlayer(Player& player, float deltaTime);

    i32 getAliveCount() const;

    i32 getZombieCount() const;
    Vector2 getZombiePosition(i32 index) const;
    i32 getZombieHealth(i32 index) const;
    void damageZombie(i32 index, i32 dmg);

    std::vector<i32> getKilledSinceLastUpdate(); 

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