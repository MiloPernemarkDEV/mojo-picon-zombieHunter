#include "Sheep.h"
#include <vector>
#include <iostream>

#include "SpriteFilePaths.h"


Sheep::Sheep()
	: m_currentSheepCount(0), m_playerPos{0}, m_fleeSpeed(2), m_texture(0), m_wanderSpeed(1)
{
}

Sheep::~Sheep()
{
}

void Sheep::update(float deltaTime)
{
    drawSprite();
	moveSheep();
}

void Sheep::drawSprite()
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

void Sheep::moveSheep()
{
    for (i32 i = 0; i < m_currentSheepCount; i++) {
        SheepStates state = stateMachine(i, GetFrameTime());

        switch (state) {
        case SheepStates::FLEEING:
            Vector2 fleeDir = fleeDirection(i);
            m_sheepPositions[i].x += fleeDir.x * m_fleeSpeed;
            m_sheepPositions[i].y += fleeDir.y * m_fleeSpeed;
            break;
        case SheepStates::IDLE:

            break;
        case SheepStates::GRAZING:

            m_sheepPositions[i].x += m_sheepDirections[i].x * m_wanderSpeed;
            m_sheepPositions[i].y += m_sheepDirections[i].y * m_wanderSpeed;
            break;
        }
    }
}

void Sheep::start()
{
    m_texture = LoadTexture(SFP::SHEEP_PNG);
    SetTextureFilter(m_texture, TEXTURE_FILTER_POINT);

    m_currentSheepCount = START_SHEEP_COUNT;

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

SheepStates Sheep::stateMachine(i32 index, float deltaTime)
{
    float maxDistSquared = MAX_DISTANCE_TO_PLAYER * MAX_DISTANCE_TO_PLAYER * static_cast<float>(GetRandomValue(0, 1));
    float distance = m_math.squaredDistance(m_sheepPositions[index], m_playerPos);

    if (distance <= maxDistSquared)
    {
        m_sheepStates[index] = SheepStates::FLEEING;
        m_sheepStateTimers[index] = 0.5f; 
        return m_sheepStates[index];
    }

    m_sheepStateTimers[index] -= deltaTime;
    if (m_sheepStateTimers[index] <= 0.0f)
    {
        i32 randomTicket = GetRandomValue(0, 2000);
        i32 middleValue = 1000;
        if (randomTicket <= middleValue)
            m_sheepStates[index] = SheepStates::IDLE;
        else
            m_sheepStates[index] = SheepStates::GRAZING;

        m_sheepStateTimers[index] = static_cast<float>(GetRandomValue(1, 5));
    }

    return m_sheepStates[index];
}

Vector2 Sheep::fleeDirection(i32 i)
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
