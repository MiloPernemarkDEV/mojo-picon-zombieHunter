#pragma once

#include "Item.h"
#include "Defines.h"
#include "Player.h"
#include <vector>
#include <memory>

class Item_health : public Item {
public:
	Item_health(Player& m_player);
	void drop(i32 amount) override;
	void start() override;
	void update(float deltaTime) override;

private:
	i32 m_healAmount;
	Player* m_player;
	const std::string& m_id;
	std::vector<std::unique_ptr<Item_health>> item_health_pool;
	
};