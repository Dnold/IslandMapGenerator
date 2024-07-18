#pragma once
#include "BaseItem.h"

class Chestplate : public BaseItem
{
public:
	Chestplate(int id, std::string name, std::string desc, float weight, int price) : BaseItem(id, name, desc, weight, price, ItemType::ARMOR,0)
	{
	}	
};

