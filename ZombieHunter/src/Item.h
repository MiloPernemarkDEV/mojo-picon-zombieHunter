#pragma once

#include <string>
#include "Defines.h"


const std::string ITEM_ARMOR_PLATE_ID = "1f2bf627-5b6d-48ec-81d3-7a3a81bb04d2";
const std::string ITEM_DARK_ENERGY_ORB = "8b997509-9ff5-490c-bf4e-e910337f9997";
const std::string ITEM_HEALING_POT = "a22a9051-2f96-4e9c-a189-c9ed6815f80a";

class Item {
public:
	virtual void drop(i32 amount) = 0;
	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;

	std::string item_id;
private:
	i32 pickup_xp;
};