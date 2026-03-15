#pragma once

#include "Item.h"
#include "Defines.h"
#include "raylib.h"
#include <vector>
#include <memory>

class Item_health : public Item {
public:
	Item_health();
	void drop(const Vector2& drop_location) override;
	void start() override;
	void update(float deltaTime) override;

private:
	const std::string& id;
	std::vector<std::unique_ptr<Item_health>> item_health_pool;
	
};