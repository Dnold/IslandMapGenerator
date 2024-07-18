#include "TreasureChest.h"
TreasureChest::TreasureChest(int id, Rectangle rect, int amountOfItems)
{
	itemContainer = new ItemContainer(amountOfItems);
	possibleItems = std::vector<std::shared_ptr<BaseItem>>();
	SetPossibleItems();
	SetRandomItems();

}
void TreasureChest::SetRandomItems()
{
	for (int i = 0; i < itemContainer->maxItems; i++)
	{
		itemContainer->AddItem(possibleItems[rand() % possibleItems.size()]);
	}
}
void TreasureChest::SetPossibleItems()
{
	possibleItems.push_back(std::make_shared<BaseItem>(Ring(0, "Gold Ring", "4000 Karates or so idk i'm not a smith", 0.1f, 20000,0.75f)));
	possibleItems.push_back(std::make_shared<BaseItem>(Ring(1, "Silver Ring", "Almost as cool as gold", 0.2f, 10000,0.45f)));
	possibleItems.push_back(std::make_shared<BaseItem>(Ring(2, "Bronze Ring", "It's not gold or silver but it's still a ring", 0.3f, 5000,0.25f)));
	possibleItems.push_back(std::make_shared<BaseItem>(Sword(3, "Broken Sword", "is broke", 5.0f, 100)));
	possibleItems.push_back(std::make_shared<BaseItem>(Sword(4, "Rusty Sword", "It's rusty, but type safe", 4.0f, 200)));
	possibleItems.push_back(std::make_shared<BaseItem>(Sword(5, "Iron Sword", "Be careful around magnets!", 3.0f, 300)));
	possibleItems.push_back(std::make_shared<BaseItem>(Sword(6, "Ultimate Sword", "Wouldn't want to be cut by that", 2.0f, 5000)));
	possibleItems.push_back(std::make_shared<BaseItem>(Chestplate(7, "Leather Armor", "Poor Cow :(", 1.2f, 75)));
	possibleItems.push_back(std::make_shared<BaseItem>(Chestplate(8, "Iron Armor", "It's heavy", 20.0f, 150)));
	possibleItems.push_back(std::make_shared<BaseItem>(Chestplate(9, "Diamond Armor", "It's shiny", 0.5f, 10000000)));
	possibleItems.push_back(std::make_shared<BaseItem>(Chestplate(10, "Ultimate Armor", "It's the best", 15.0f, 1000000000)));
}
std::vector<std::shared_ptr<BaseItem>> TreasureChest::GetItems()
{
	std::vector<std::shared_ptr<BaseItem>> items;
	for (int i = 0; i < itemContainer->maxItems; i++)
	{
		items.push_back(itemContainer->GetItem<std::shared_ptr<BaseItem>>(i));
	}
	return items;
}