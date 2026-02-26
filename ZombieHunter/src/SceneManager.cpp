#include "SceneManager.h"

void SceneManager::start() {

	addSauce();

	for (i32 i = 0; i < sauces.size(); i++) {
		sauces[i]->start();
	}
}

void SceneManager::update(float deltaTime) {

	m_Sheep.m_playerPos = m_Player.getPlayerPosition();

	for (i32 i = 0; i < sauces.size(); i++) {
		sauces[i]->update(deltaTime);
	}
}

void SceneManager::addSauce()
{
	sauces.push_back(&m_FrameTool);
	sauces.push_back(&m_Player);
	sauces.push_back(&m_Sheep);
}
