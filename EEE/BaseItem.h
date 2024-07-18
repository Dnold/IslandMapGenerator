#pragma once
#include <raylib.h>
#include <string>
class BaseItem
{
	public:
	enum class ItemType
	{
		WEAPON,
		ARMOR,
		RING,
		CONSUMABLE,
		QUEST,
		OTHER
	};

	int id;
	std::string name;
	std::string description;
	float weight;
	int price;
	ItemType type;
	float strength;

	BaseItem(int id, std::string name, std::string description, float weight, int price, ItemType type,float strength);
};


