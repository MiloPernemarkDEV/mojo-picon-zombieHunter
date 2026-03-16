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
        m_position{ 720, 400 },
        m_speed{ 0 },
        m_walkSpeed{ 200.0f },
        m_sprintSpeed{ 300.0f },
        m_frameCount{ 8 },
        m_frameWidth{ 0 },
        m_frameHeight{ 0 },
        m_frameIndex{ 0 },
        m_maxHealth{ 100 },
        m_currentHealth{ 100 },
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

        playerInput(deltaTime);

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
            primary_attack();

        if (m_secondaryAttackTriggered)
            secondary_attack();

        passive_attack();

        draw_sprite();
    }

    void Player::playerInput(float deltaTime)
    {
        m_speed = IsKeyDown(KEY_LEFT_SHIFT) ? m_sprintSpeed : m_walkSpeed;

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

            m_position.x += moveDir.x * m_speed * deltaTime;
            m_position.y += moveDir.y * m_speed * deltaTime;
        }

        if (IsKeyPressed(KEY_Q))
        {
            if (m_currentWeapon == WeaponType::MoveDirection)
                m_currentWeapon = WeaponType::MouseTarget;
            else
                m_currentWeapon = WeaponType::MoveDirection;
        }
    }

    void Player::primary_attack()
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

        for (i32 i = 0; i < zombieManager.get_zombie_count(); i++)
        {
            Vector2 zombiePos = zombieManager.get_zombie_position(i);
            float distSq = MojoPiconMath::squared_distance(target, zombiePos);

            if (distSq <= effect.maxRadius * effect.maxRadius)
                zombieManager.damage_zombie(i, 10);
        }
    }

    void Player::secondary_attack()
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

        for (i32 i = 0; i < zombieManager.get_zombie_count(); i++)
        {
            Vector2 zombiePos = zombieManager.get_zombie_position(i);
            float distSq = MojoPiconMath::squared_distance(target, zombiePos);

            if (distSq <= effect.maxRadius * effect.maxRadius)
                zombieManager.damage_zombie(i, 15);
        }
    }

    void Player::add_health(i32 amount)
    {
        if (amount <= 0) return;
        m_currentHealth += amount;
    }

    void Player::passive_attack()
    {
        Vector2 playerPos = get_player_position();

        for (i32 i = 0; i < zombieManager.get_zombie_count(); i++)
        {
            Vector2 zombiePos = zombieManager.get_zombie_position(i);

            if (zombieManager.get_zombie_health(i) <= 0)
                continue;

            float distSq = MojoPiconMath::squared_distance(playerPos, zombiePos);

            if (distSq <= PASSIVE_ATTACK_RANGE * PASSIVE_ATTACK_RANGE)
            {
                zombieManager.damage_zombie(i, 1 + get_passive_bonus());

                PassiveHitEffect effect;
                effect.position = zombiePos;
                effect.timer = PASSIVE_HIT_DURATION;

                g_passiveHitEffects.push_back(effect);
            }
        }
    }

    void Player::draw_sprite()
    {
        Rectangle sourceRect =
        {
            (float)m_frameIndex * m_frameWidth,
            0,
            (float)m_frameWidth,
            (float)m_frameHeight
        };

        float scale = 3.0f;

        Rectangle destRect =
        {
            m_position.x,
            m_position.y,
            m_frameWidth * scale,
            m_frameHeight * scale
        };

        Vector2 origin =
        {
            (m_frameWidth * scale) * 0.5f,
            m_frameHeight * scale
        };

        DrawTexturePro(m_spriteSheet, sourceRect, destRect, origin, 0.0f, WHITE);

        DrawCircleV(m_position, 4, BLUE);

        draw_active_effects();
        draw_passive_effects();
        draw_health_bar();
    }

    void Player::draw_passive_effects()
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

    void Player::draw_active_effects()
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

    void Player::draw_health_bar()
    {
        const float barWidth = 100.0f;
        const float barHeight = 10.0f;
        const float offsetY = 70.0f;

        float healthPercent = (float)m_currentHealth / (float)m_maxHealth;

        Vector2 barPos =
        {
            m_position.x - barWidth / 2,
            m_position.y - offsetY
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
