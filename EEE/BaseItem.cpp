#include "BaseItem.h"
BaseItem::BaseItem(int id, std::string name, std::string description, float weight, int price, ItemType type,float strength)
{
	this->id = id;
	this->name = name;
	this->description = description;
	this->weight = weight;
	this->price = price;
	this->type = type;
	this->strength = strength;
}