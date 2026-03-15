#pragma once

#include "Item.h"
#include <unordered_map>
#include <string>
#include <memory>

class Inventory {
public:
    void add_item(std::unique_ptr<Item> item);
    Item* get_item(const std::string& id); 
    void remove_item(const std::string& id);
    void clear();
    const std::unordered_map<std::string, std::unique_ptr<Item>>& get_inv() const { return item_inventory; };

private:
    std::unordered_map<std::string, std::unique_ptr<Item>> item_inventory;
};