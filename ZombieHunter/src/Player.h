#pragma once

#include "SauceMaker.h"
#include "Defines.h"
#include <stdint.h>
#include "Zombie.h"
#include <vector>
#include <raylib.h>

struct ActiveAttackEffect
{
    Vector2 position;
    float radius;
    float maxRadius;
    float timer;
    float duration;
    Color color;
};

struct Player : public SauceMaker
{
public:
    Player(Zombie& zombieManager);
    ~Player();

    void update(float deltaTime) override;
    void drawSprite() override;
    void playerInput();
    void start() override;

    Vector2 get_player_position() { return m_position; }
    i32 get_current_health() const { return m_currentHealth; }
    void take_damage(i32 amount) { m_currentHealth = std::max(0, m_currentHealth - amount); }
    bool is_primary_attack_triggered() const { return m_primaryAttackTriggered; }
    bool is_secondary_attack_triggered() const { return m_secondaryAttackTriggered; }
    void add_passive_kill() { m_passiveKills++; }
    i32 get_passive_bonus() const { return m_passiveKills / 10; }

private:
    void passive_attack();
    void primary_attack();
    void secondary_attack();
    void draw_passive_effects();
    void draw_active_effects();
    void draw_health_bar();

private:
    Texture2D m_spriteSheet;

    i32 m_frameCount;
    i32 m_frameWidth;
    i32 m_frameIndex;
    i32 m_frameHeight;

    float m_speed;
    float m_walkSpeed;
    float m_sprintSpeed;

    Vector2 m_position;

    i32 m_maxHealth;
    i32 m_currentHealth;

    bool m_primaryAttackTriggered;
    bool m_secondaryAttackTriggered;

    float m_primaryAttackVisualTime;
    float m_secondaryAttackVisualTime;

    Zombie& zombieManager;

    i32 m_passiveKills{ 0 };

    enum class WeaponType { MoveDirection, MouseTarget };
    WeaponType m_currentWeapon = WeaponType::MoveDirection;
    float m_fireRatePrimary = 0.3f;  
    float m_fireTimerPrimary = 0.0f;
    float m_fireRateSecondary = 0.6f;
    float m_fireTimerSecondary = 0.0f;

    static std::vector<ActiveAttackEffect> g_activeEffects;
};