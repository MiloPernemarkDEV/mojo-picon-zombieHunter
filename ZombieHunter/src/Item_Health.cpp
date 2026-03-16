#include "Item_health.h"
#include "GameManager.h"
#include <memory>


Item_health::Item_health(Player& player)
	: m_id(ITEM_HEALING_POT), m_player(&player), m_healAmount(20)
{
}

void Item_health::drop(i32 amount)
{
	GameManager::Instance().add_item_to_count(amount);
	if (m_player == nullptr) return;
	m_player->add_health(m_healAmount);
}

void Item_health::start()
{

}

void Item_health::update(float deltaTime)
{

}


