// SceneManager will call all special functions inside application.cpp

#pragma once

#include "Defines.h"
#include <raylib.h>
#include <vector>
#include "SauceMaker.h"
#include "Player.h"
#include "Sheep.h"
#include "FrameTool.h"

struct SceneManager {
public:
	void start();
	void update();
private:
	std::vector<SauceMaker*> sauces;
	Player m_Player;
	Sheep m_Sheep;
	FrameTool m_FrameTool;

	void addSauce();
};