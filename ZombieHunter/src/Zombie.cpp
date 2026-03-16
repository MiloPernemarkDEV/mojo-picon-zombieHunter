#include "Zombie.h"

#include <cmath>
#include <iostream>

#include "MojoPiconMath.h"
#include "Player.h"

struct AttackEffect
{
    Vector2 position;
    float timer; 
};
static std::vector<AttackEffect> g_attackEffects;
const float ATTACK_EFFECT_DURATION = 0.2f;

Zombie::Zombie()
    : m_walkerSpeed{ 1 }
    , m_runnerSpeed{ 2 }
    , m_currentZombieCount{ 0 }
    , m_playerPos{ 0, 0 }
    , m_zombieRadius{ 15.0f }
    , m_attackRange{ 20.0f }
    , m_attackTimer{ 0.0f }
    , m_attackCooldown{ 0.5f }
{
}

Zombie::~Zombie()
{
}

void Zombie::start()
{
    m_currentZombieCount = 0;
    m_zombiePositions.clear();
    m_zombieTypes.clear();
    m_zombieHealth.clear();
}

void Zombie::update(float deltaTime)
{
    move_zombies(deltaTime);
    draw_sprite();
}

void Zombie::draw_sprite()
{
    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] <= 0)
            continue;

        DrawCircleV(m_zombiePositions[i], m_zombieRadius, RED);

        const float barWidth = 30.0f;
        const float barHeight = 5.0f;
        const float offsetY = 25.0f; 
        float maxHealth = (m_zombieTypes[i] == ZombieType::WALKER ? WALKER_ZOMBIE_HEALTH : RUNNER_ZOMBIE_HEALTH);
        float healthPercent = static_cast<float>(m_zombieHealth[i]) / maxHealth;
        Vector2 barPos = { m_zombiePositions[i].x - barWidth / 2, m_zombiePositions[i].y - offsetY };

        DrawRectangle(static_cast<int>(barPos.x), static_cast<int>(barPos.y),
            static_cast<int>(barWidth), static_cast<int>(barHeight), GRAY);
        Color healthColor = {
            static_cast<unsigned char>((1.0f - healthPercent) * 255),
            static_cast<unsigned char>(healthPercent * 255),
            0, 255 };
        DrawRectangle(static_cast<int>(barPos.x), static_cast<int>(barPos.y),
            static_cast<int>(barWidth * healthPercent), static_cast<int>(barHeight), healthColor);
    }

    for (i32 i = (i32)g_attackEffects.size() - 1; i >= 0; i--)
    {
        AttackEffect& effect = g_attackEffects[i];
        DrawCircleV(effect.position, 30.0f, Color{ 255, 100, 100, 150 }); 
        effect.timer -= GetFrameTime();
        if (effect.timer <= 0.0f)
        {
            g_attackEffects.erase(g_attackEffects.begin() + i);
        }
    }
}

void Zombie::move_zombies(float deltaTime)
{
    const float separationDistance = m_zombieRadius * 2.0f;
    const float separationStrength = 0.5f;

    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] <= 0)
            continue;

        Vector2 directionToPlayer = { m_playerPos.x - m_zombiePositions[i].x, m_playerPos.y - m_zombiePositions[i].y };
        float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
        if (length != 0.0f)
        {
            directionToPlayer.x /= length;
            directionToPlayer.y /= length;
        }

        Vector2 separation{ 0, 0 };
        for (i32 j = 0; j < m_currentZombieCount; j++)
        {
            if (i == j || m_zombieHealth[j] <= 0)
                continue;

            Vector2 diff = { m_zombiePositions[i].x - m_zombiePositions[j].x,
                             m_zombiePositions[i].y - m_zombiePositions[j].y };
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (dist < separationDistance && dist > 0.0f)
            {
                separation.x += diff.x / dist * (separationDistance - dist) * separationStrength;
                separation.y += diff.y / dist * (separationDistance - dist) * separationStrength;
            }
        }

        Vector2 finalDir = { directionToPlayer.x + separation.x, directionToPlayer.y + separation.y };
        float finalLen = std::sqrt(finalDir.x * finalDir.x + finalDir.y * finalDir.y);
        if (finalLen != 0.0f)
        {
            finalDir.x /= finalLen;
            finalDir.y /= finalLen;
        }

        const ZombieType type = m_zombieTypes[i];
        const float speed = (type == ZombieType::WALKER ? static_cast<float>(m_walkerSpeed) : static_cast<float>(m_runnerSpeed));
        m_zombiePositions[i].x += finalDir.x * speed * deltaTime * 60.0f;
        m_zombiePositions[i].y += finalDir.y * speed * deltaTime * 60.0f;
    }
}

void Zombie::apply_primary_attack(Vector2 playerPos)
{
    const float range = 60.0f;
    const float rangeSq = range * range;

    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] <= 0)
            continue;

        const float distSq = MojoPiconMath::squared_distance(m_zombiePositions[i], playerPos);
        if (distSq <= rangeSq)
            m_zombieHealth[i] -= 1;
    }
}

void Zombie::apply_secondary_attack(Vector2 playerPos)
{
    const float range = 100.0f;
    const float rangeSq = range * range;

    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] <= 0)
            continue;

        const float distSq = MojoPiconMath::squared_distance(m_zombiePositions[i], playerPos);
        if (distSq <= rangeSq)
            m_zombieHealth[i] -= 3;
    }
}

void Zombie::start_wave(i32 waveIndex, Vector2 center)
{
    const i32 baseCount = START_ZOMBIE_COUNT;
    const i32 incrementPerWave = 5;
    m_currentZombieCount = baseCount + (waveIndex - 1) * incrementPerWave;
    if (m_currentZombieCount > MAX_ZOMBIE_COUNT)
        m_currentZombieCount = MAX_ZOMBIE_COUNT;
    if (m_currentZombieCount < 0)
        m_currentZombieCount = 0;

    m_zombiePositions.resize(m_currentZombieCount);
    m_zombieTypes.resize(m_currentZombieCount);
    m_zombieHealth.resize(m_currentZombieCount);

    const float spawnRadius = 350.0f;
    const float radiusJitter = 75.0f;
    const float deg2rad = 3.1415926f / 180.0f;

    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        i32 angleDeg = GetRandomValue(0, 359);
        float angle = static_cast<float>(angleDeg) * deg2rad;
        float radius = spawnRadius + static_cast<float>(GetRandomValue(static_cast<int>(-radiusJitter), static_cast<int>(radiusJitter)));

        m_zombiePositions[i].x = center.x + std::cos(angle) * radius;
        m_zombiePositions[i].y = center.y + std::sin(angle) * radius;

        i32 typeTicket = GetRandomValue(0, 1);
        m_zombieTypes[i] = typeTicket == 0 ? ZombieType::WALKER : ZombieType::RUNNER;
        m_zombieHealth[i] = (m_zombieTypes[i] == ZombieType::WALKER ? WALKER_ZOMBIE_HEALTH : RUNNER_ZOMBIE_HEALTH);
    }
}

i32 Zombie::get_alive_count() const
{
    i32 alive = 0;
    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] > 0)
            alive++;
    }
    return alive;
}

void Zombie::attack_player(Player& player, float deltaTime)
{
    m_attackTimer += deltaTime;
    Vector2 playerPos = player.get_player_position();
    const float attackRangeTotal = m_zombieRadius + m_attackRange;
    const float attackRangeSq = attackRangeTotal * attackRangeTotal;

    for (i32 i = 0; i < m_currentZombieCount; i++)
    {
        if (m_zombieHealth[i] <= 0)
            continue;

        float distSq = MojoPiconMath::squared_distance(m_zombiePositions[i], playerPos);
        if (distSq <= attackRangeSq && m_attackTimer >= m_attackCooldown)
        {
            player.take_damage(10);
            m_attackTimer = 0.0f;

            AttackEffect effect;
            effect.position = playerPos;
            effect.timer = ATTACK_EFFECT_DURATION;
            g_attackEffects.push_back(effect);

            break;
        }
    }
}

void Zombie::damage_zombie(i32 index, i32 dmg)
{
    if (index >= 0 && index < m_currentZombieCount)
    {
        i32 prevHealth = m_zombieHealth[index];
        m_zombieHealth[index] -= dmg;

        if (prevHealth > 0 && m_zombieHealth[index] <= 0)
        {
            m_recentlyKilled.push_back(index);
        }
    }
}

i32 Zombie::get_zombie_count() const
{
    return m_currentZombieCount; 
}

std::vector<i32> Zombie::get_killed_since_last_update()
{
    std::vector<i32> killed = m_recentlyKilled;
    m_recentlyKilled.clear();
    return killed;
}
