#include "Sheep.h"
#include <vector>
#include <iostream>

#include "SpriteFilePaths.h"


Sheep::Sheep()
	: m_currentSheepCount(0), m_playerPos{0}, m_speed(2), m_texture(0)
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
    float maxDistSquared = MAX_DISTANCE_TO_PLAYER * MAX_DISTANCE_TO_PLAYER;
    for (i32 i = 0; i < m_currentSheepCount; i++)
    {
        float distance = m_math.squaredDistance(m_sheepPositions[i], m_playerPos);

        if (distance <= maxDistSquared)
        {
            m_sheepPositions[i].x += m_sheepDirections[i].x * m_speed;
            m_sheepPositions[i].y += m_sheepDirections[i].y * m_speed;
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

    for (i32 i = 0; i < m_currentSheepCount; i++)
    {
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






