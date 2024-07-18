#pragma once
#include "BaseItem.h"

class Sword : public BaseItem
{
public:
	Sword(int id, std::string name, std::string desc, float weight, int price) : BaseItem(id, name, desc, weight, price,ItemType::WEAPON,0)
	{
	};
};

