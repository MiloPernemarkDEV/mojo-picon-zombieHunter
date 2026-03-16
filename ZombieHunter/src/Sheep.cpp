#include "Sheep.h"
#include <vector>
#include <iostream>

#include "SpriteFilePaths.h"


Sheep::Sheep()
	: m_currentSheepCount(0), m_playerPos{0}, m_fleeSpeed(150), m_texture(0), m_wanderSpeed(70), m_hasSpawned(false)
{
}

Sheep::~Sheep()
{
}

void Sheep::update(float deltaTime)
{
    draw_sprite();
	move_sheep(deltaTime);
}

void Sheep::draw_sprite()
{
	for (i32 i = 0; i < m_currentSheepCount; i++) {
        float scale = 0.03f;
        DrawTextureEx(
            m_texture,
            m_sheepPositions[i], 
            0.0f,                 
            scale,                
            WHITE
        );
	}
}

void Sheep::move_sheep(float deltaTime)
{
    for (i32 i = 0; i < m_currentSheepCount; i++) {
        SheepStates state = state_machine(i, GetFrameTime());

        switch (state) {
        case SheepStates::FLEEING:
            Vector2 fleeDir = flee_direction(i);
            m_sheepPositions[i].x += fleeDir.x * m_fleeSpeed * deltaTime;
            m_sheepPositions[i].y += fleeDir.y * m_fleeSpeed * deltaTime;
            break;
        case SheepStates::IDLE:

            break;
        case SheepStates::GRAZING:

            m_sheepPositions[i].x += m_sheepDirections[i].x * m_wanderSpeed * deltaTime;
            m_sheepPositions[i].y += m_sheepDirections[i].y * m_wanderSpeed * deltaTime;
            break;
        }
    }
}

void Sheep::start()
{
    if (m_texture.id == 0) {
        m_texture = LoadTexture(SFP::SHEEP_PNG);
        SetTextureFilter(m_texture, TEXTURE_FILTER_POINT);
    }

    if (START_SHEEP_COUNT > MAX_SHEEP_COUNT) {
        m_currentSheepCount = MAX_SHEEP_COUNT;
    }
    else {
        m_currentSheepCount = START_SHEEP_COUNT;
    }

   
    if (!m_hasSpawned)
    {
        m_sheepPositions.resize(m_currentSheepCount);
        m_sheepDirections.resize(m_currentSheepCount);
        m_sheepStates.resize(m_currentSheepCount);
        m_sheepStateTimers.resize(m_currentSheepCount);

        for (i32 i = 0; i < m_currentSheepCount; i++)
        {
            m_sheepStates[i] = SheepStates::GRAZING;
            m_sheepStateTimers[i] = static_cast<float>(GetRandomValue(1, 5));

            m_sheepPositions[i].x = static_cast<float>(GetRandomValue(0, 800));
            m_sheepPositions[i].y = static_cast<float>(GetRandomValue(0, 900));

            m_sheepDirections[i].x = static_cast<float>(GetRandomValue(-100, 100));
            m_sheepDirections[i].y = static_cast<float>(GetRandomValue(-100, 100));

            float length = sqrt(
                m_sheepDirections[i].x * m_sheepDirections[i].x +
                m_sheepDirections[i].y * m_sheepDirections[i].y
            );

            if (length != 0)
            {
                m_sheepDirections[i].x /= length;
                m_sheepDirections[i].y /= length;
            }
        }
    }
    else
    {

        for (i32 i = 0; i < m_currentSheepCount; i++)
        {
            if (m_sheepStateTimers[i] <= 0.0f) {
                m_sheepStateTimers[i] = static_cast<float>(GetRandomValue(1, 5));
            }
        }
    }

    m_hasSpawned = true;
}

SheepStates Sheep::state_machine(i32 i, float deltaTime)
{
    float maxDistSquared = MAX_DISTANCE_TO_PLAYER * MAX_DISTANCE_TO_PLAYER * static_cast<float>(GetRandomValue(0, 1));
    float distance = MojoPiconMath::squared_distance(m_sheepPositions[i], m_playerPos);

    if (distance <= maxDistSquared)
    {
        m_sheepStates[i] = SheepStates::FLEEING;
        m_sheepStateTimers[i] = 0.5f; 
        return m_sheepStates[i];
    }

    m_sheepStateTimers[i] -= deltaTime;
    if (m_sheepStateTimers[i] <= 0.0f)
    {
        i32 randomTicket = GetRandomValue(0, 2000);
        i32 middleValue = 1000;
        if (randomTicket <= middleValue)
            m_sheepStates[i] = SheepStates::IDLE;
        else
            m_sheepStates[i] = SheepStates::GRAZING;

        m_sheepStateTimers[i] = static_cast<float>(GetRandomValue(1, 5));
    }

    return m_sheepStates[i];
}

void Sheep::set_has_spawned(bool hasSpawned)
{
    m_hasSpawned = hasSpawned;
}

void Sheep::set_loaded_positions(const std::vector<Vector2>& positions)
{
    m_sheepPositions = positions;
    m_currentSheepCount = static_cast<i32>(positions.size());

    m_sheepDirections.resize(m_currentSheepCount);
    m_sheepStates.resize(m_currentSheepCount);
    m_sheepStateTimers.resize(m_currentSheepCount);
}

Vector2 Sheep::flee_direction(i32 i)
{
    Vector2 fleeDir;
    fleeDir.x = m_sheepPositions[i].x - m_playerPos.x;
    fleeDir.y = m_sheepPositions[i].y - m_playerPos.y;

    float length = sqrt(fleeDir.x * fleeDir.x + fleeDir.y * fleeDir.y);
    if (length != 0)
    {
        fleeDir.x /= length;
        fleeDir.y /= length;
    }

    return fleeDir;
}