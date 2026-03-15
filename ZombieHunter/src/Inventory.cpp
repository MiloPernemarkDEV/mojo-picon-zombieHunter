#include "Inventory.h"

void Inventory::add_item(std::unique_ptr<Item> item) {
    if (item) {
        
        item_inventory[item->item_id] = std::move(item);
    }
}

Item* Inventory::get_item(const std::string& id) {
    auto it = item_inventory.find(id);
    if (it != item_inventory.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Inventory::remove_item(const std::string& id) {
    item_inventory.erase(id);
}

void Inventory::clear() {
    item_inventory.clear();
}