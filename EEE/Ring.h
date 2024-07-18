#pragma once
#include "BaseItem.h"

class Ring :public BaseItem
{
public:
	Ring(int id,std::string name, std::string desc, float weight,int price,float strength) : BaseItem(id,name,desc,weight,price,ItemType::RING,strength) {};
};

