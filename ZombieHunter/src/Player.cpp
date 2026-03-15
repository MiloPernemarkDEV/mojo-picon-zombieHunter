    #include "Player.h"
    #include <iostream>
    #include "SpriteFilePaths.h"
    #include "MojoPiconMath.h"
    #include "raymath.h"   

    struct PassiveHitEffect
    {
        Vector2 position;
        float timer;
    };

    static std::vector<PassiveHitEffect> g_passiveHitEffects;
    std::vector<ActiveAttackEffect> Player::g_activeEffects;

    const float PASSIVE_HIT_DURATION = 0.15f;
    const float PASSIVE_ATTACK_RANGE = 100.0f;

    const float PRIMARY_ATTACK_RANGE = 30.0f;
    const float SECONDARY_ATTACK_RANGE = 50;

    Player::Player(Zombie& zombieManager)
        : m_spriteSheet{},
        zombieManager(zombieManager),
        position{ 720, 400 },
        speed{ 0 },
        walkSpeed{ 2.7f },
        sprintSpeed{ 8.0f },
        frameCount{ 8 },
        frameWidth{ 0 },
        frameHeight{ 0 },
        frameIndex{ 0 },
        maxHealth{ 100 },
        currentHealth{ 100 },
        m_primaryAttackTriggered{ false },
        m_secondaryAttackTriggered{ false },
        m_primaryAttackVisualTime{ 0.0f },
        m_secondaryAttackVisualTime{ 0.0f },
        m_fireRatePrimary{ 0.02f },    
        m_fireRateSecondary{ 0.1f },  
        m_fireTimerPrimary{ 0.0f },
        m_fireTimerSecondary{ 0.0f }
    {
    }

    Player::~Player() {}

    void Player::start() {}

    void Player::update(float deltaTime)
    {
        m_primaryAttackTriggered = false;
        m_secondaryAttackTriggered = false;

        playerInput();

        if (m_fireTimerPrimary > 0.0f) m_fireTimerPrimary -= deltaTime;
        if (m_fireTimerSecondary > 0.0f) m_fireTimerSecondary -= deltaTime;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m_fireTimerPrimary <= 0.0f)
        {
            m_primaryAttackTriggered = true;
            m_fireTimerPrimary = m_fireRatePrimary;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && m_fireTimerSecondary <= 0.0f)
        {
            m_secondaryAttackTriggered = true;
            m_fireTimerSecondary = m_fireRateSecondary;
        }

        if (m_primaryAttackTriggered)
            primaryAttack();

        if (m_secondaryAttackTriggered)
            secondaryAttack();

        passiveAttack();

        drawSprite();
    }

    void Player::playerInput()
    {
        speed = IsKeyDown(KEY_LEFT_SHIFT) ? sprintSpeed : walkSpeed;

        Vector2 moveDir = { 0, 0 };
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) moveDir.y -= 1;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) moveDir.y += 1;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveDir.x -= 1;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDir.x += 1;

        if (moveDir.x != 0 || moveDir.y != 0)
        {
            float len = Vector2Length(moveDir);
            moveDir.x /= len;
            moveDir.y /= len;

            position.x += moveDir.x * speed;
            position.y += moveDir.y * speed;
        }

        if (IsKeyPressed(KEY_Q))
        {
            if (m_currentWeapon == WeaponType::MoveDirection)
                m_currentWeapon = WeaponType::MouseTarget;
            else
                m_currentWeapon = WeaponType::MoveDirection;
        }
    }

    void Player::primaryAttack()
    {
        Vector2 target = GetMousePosition();

        ActiveAttackEffect effect;
        effect.position = target;
        effect.radius = 0;

        if (m_currentWeapon == WeaponType::MoveDirection) {
            effect.maxRadius = PRIMARY_ATTACK_RANGE;
            effect.color = YELLOW;
            effect.duration = 0.35f;
        }
        else {
            effect.maxRadius = PRIMARY_ATTACK_RANGE * 1.5f;
            effect.color = GREEN;
            effect.duration = 0.5f;
        }

        effect.timer = effect.duration;  

        g_activeEffects.push_back(effect);

        for (i32 i = 0; i < zombieManager.getZombieCount(); i++)
        {
            Vector2 zombiePos = zombieManager.getZombiePosition(i);
            float distSq = MojoPiconMath::squaredDistance(target, zombiePos);

            if (distSq <= effect.maxRadius * effect.maxRadius)
                zombieManager.damageZombie(i, 10);
        }
    }

    void Player::secondaryAttack()
    {
        Vector2 target = GetMousePosition();

        ActiveAttackEffect effect;
        effect.position = target;
        effect.radius = 0;
        effect.timer = effect.duration = 0.6f;

        if (m_currentWeapon == WeaponType::MoveDirection)
        {
            effect.maxRadius = SECONDARY_ATTACK_RANGE;
            effect.color = ORANGE;
        }
        else 
        {
            effect.maxRadius = SECONDARY_ATTACK_RANGE * 1.5f; 
            effect.color = BLUE; 
        }

        g_activeEffects.push_back(effect);

        for (i32 i = 0; i < zombieManager.getZombieCount(); i++)
        {
            Vector2 zombiePos = zombieManager.getZombiePosition(i);
            float distSq = MojoPiconMath::squaredDistance(target, zombiePos);

            if (distSq <= effect.maxRadius * effect.maxRadius)
                zombieManager.damageZombie(i, 15);
        }
    }

    void Player::passiveAttack()
    {
        Vector2 playerPos = getPlayerPosition();

        for (i32 i = 0; i < zombieManager.getZombieCount(); i++)
        {
            Vector2 zombiePos = zombieManager.getZombiePosition(i);

            if (zombieManager.getZombieHealth(i) <= 0)
                continue;

            float distSq = MojoPiconMath::squaredDistance(playerPos, zombiePos);

            if (distSq <= PASSIVE_ATTACK_RANGE * PASSIVE_ATTACK_RANGE)
            {
                zombieManager.damageZombie(i, 1 + getPassiveBonus());

                PassiveHitEffect effect;
                effect.position = zombiePos;
                effect.timer = PASSIVE_HIT_DURATION;

                g_passiveHitEffects.push_back(effect);
            }
        }
    }

    void Player::drawSprite()
    {
        Rectangle sourceRect =
        {
            (float)frameIndex * frameWidth,
            0,
            (float)frameWidth,
            (float)frameHeight
        };

        float scale = 3.0f;

        Rectangle destRect =
        {
            position.x,
            position.y,
            frameWidth * scale,
            frameHeight * scale
        };

        Vector2 origin =
        {
            (frameWidth * scale) * 0.5f,
            frameHeight * scale
        };

        DrawTexturePro(m_spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);

        DrawCircleV(position, 4, BLUE);

        drawActiveEffects();
        drawPassiveEffects();
        drawHealthBar();
    }

    void Player::drawPassiveEffects()
    {
        for (i32 i = (i32)g_passiveHitEffects.size() - 1; i >= 0; i--)
        {
            PassiveHitEffect& effect = g_passiveHitEffects[i];

            DrawCircleV(effect.position, 8.0f, Color{ 255,255,0,200 });

            effect.timer -= GetFrameTime();

            if (effect.timer <= 0.0f)
                g_passiveHitEffects.erase(g_passiveHitEffects.begin() + i);
        }
    }

    void Player::drawActiveEffects()
    {
        for (i32 i = (i32)g_activeEffects.size() - 1; i >= 0; i--)
        {
            ActiveAttackEffect& effect = g_activeEffects[i];

            float progress = effect.timer / effect.duration;
            effect.radius = effect.maxRadius * (1.0f - progress);

            int thickness = (effect.color.r == YELLOW.r) ? 2 : 3;

            DrawCircleLines(
                (int)effect.position.x,
                (int)effect.position.y,
                effect.radius,
                effect.color
            );

            DrawCircleLines(
                (int)effect.position.x,
                (int)effect.position.y,
                effect.radius * 0.6f,
                effect.color
            );

            effect.timer -= GetFrameTime();

            if (effect.timer <= 0.0f)
                g_activeEffects.erase(g_activeEffects.begin() + i);
        }
    }

    void Player::drawHealthBar()
    {
        const float barWidth = 100.0f;
        const float barHeight = 10.0f;
        const float offsetY = 70.0f;

        float healthPercent = (float)currentHealth / (float)maxHealth;

        Vector2 barPos =
        {
            position.x - barWidth / 2,
            position.y - offsetY
        };

        DrawRectangle((int)barPos.x, (int)barPos.y, (int)barWidth, (int)barHeight, GRAY);

        Color healthColor =
        {
            (unsigned char)((1.0f - healthPercent) * 255),
            (unsigned char)(healthPercent * 255),
            0,
            255
        };

        DrawRectangle(
            (int)barPos.x,
            (int)barPos.y,
            (int)(barWidth * healthPercent),
            (int)barHeight,
            healthColor
        );
    }
