#pragma once
#include "BaseItem.h"
#include "Ring.h"
#include "ItemContainer.h"
#include "Sword.h"
#include "Ring.h"
#include "Chestplate.h"
class BaseItem;
class TreasureChest
{
	public:
	ItemContainer* itemContainer;

	std::vector<std::shared_ptr<BaseItem>> possibleItems;
	TreasureChest(int id, Rectangle rect, int amountOfItems);

	void SetRandomItems();
	void SetPossibleItems();
	std::vector<std::shared_ptr<BaseItem>> GetItems();
};


