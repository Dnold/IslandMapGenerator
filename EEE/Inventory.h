#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "BaseItem.h"
#include "EquipmentSlot.h"
#include "ItemContainer.h"
#include "TreasureChest.h"
class BaseItem;
class Inventory
{
	public:
	int maxItems = 10;
	ItemContainer* items;
	EquipmentSlot* equipmentSlots = new EquipmentSlot[3];
	Inventory(int _maxItems);

	template<typename T>
	void AddItem(std::shared_ptr<T> item) // Adjusted to accept std::shared_ptr<T>
	{
		items->AddItem<T>(item); // Directly pass std::shared_ptr<T>

	}
	template<typename T>
	T GetItem(int index)
	{
		return items[index];
	}

	void RemoveItem(int index);
	bool TryEquipItem(std::shared_ptr<BaseItem> item);

	void RenderItems(int startX, int startY, int lineHeight);
	void RenderEquipmentSlots(int startX, int startY, int lineheight);

	void SortInventoryWeight();
	void SortInventoryName();
	void SortInventoryPrice();

};