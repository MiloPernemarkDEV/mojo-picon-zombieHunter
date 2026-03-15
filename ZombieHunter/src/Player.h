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

    Vector2 getPlayerPosition() { return position; }
    i32 getCurrentHealth() const { return currentHealth; }
    void takeDamage(i32 amount) { currentHealth = std::max(0, currentHealth - amount); }
    bool isPrimaryAttackTriggered() const { return m_primaryAttackTriggered; }
    bool isSecondaryAttackTriggered() const { return m_secondaryAttackTriggered; }
    void addPassiveKill() { m_passiveKills++; }
    i32 getPassiveBonus() const { return m_passiveKills / 10; }

private:
    void passiveAttack();
    void primaryAttack();
    void secondaryAttack();
    void drawPassiveEffects();
    void drawActiveEffects();
    void drawHealthBar();

private:
    Texture2D m_spriteSheet;

    i32 frameCount;
    i32 frameWidth;
    i32 frameIndex;
    i32 frameHeight;

    float speed;
    float walkSpeed;
    float sprintSpeed;

    Vector2 position;

    i32 maxHealth;
    i32 currentHealth;

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