#pragma once
#include <functional>
#include <memory>
#include <iostream>

#include "BaseItem.h"

class EquipmentSlot
{
public:
	std::shared_ptr<BaseItem> itemSlot;
	BaseItem::ItemType slotType;
	EquipmentSlot() {};
	EquipmentSlot(BaseItem::ItemType type) : slotType(type) {};
	void SetSlot(std::shared_ptr<BaseItem> item);
	bool CanEquip(std::shared_ptr<BaseItem> item);
	void UnequipItem(std::function<void(std::shared_ptr<BaseItem>)> returnToInventory);
	void EquipItem(std::shared_ptr<BaseItem> item, std::function<void(std::shared_ptr<BaseItem>)> returnToInventory);

};

