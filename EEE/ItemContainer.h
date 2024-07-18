#pragma once
#include <iostream>
#include <memory>
#include <memory>
#include <vector>

#include "BaseItem.h"
class BaseItem;
class ItemContainer
{
	std::vector<std::shared_ptr<BaseItem>> itemSlots;

	public:
	int maxItems;

	ItemContainer(int maxItems);

	template<typename T>
	void AddItem(std::shared_ptr<T> item)
	{
		static_assert(std::is_base_of<BaseItem, T>::value, "T must be derived from BaseItem");

		if (itemSlots.size() < maxItems)
		{
			itemSlots.push_back(item);
			std::cout << "Item added to inventory" << std::endl;
		}
		else
		{
			std::cout << "Inventory is already full";
		}

	}

	template<typename T>
	void SetItem(int index, std::shared_ptr<T> item) {
		if (index >= 0 && index < itemSlots.size()) {
			itemSlots[index] = item;
		}
	}

	template <typename T>
	T GetItem(int index)
	{
		if (index < itemSlots.size())
		{
			return itemSlots[index];
		}
		else
		{
			std::cout << "Index out of bounds";
		}

	}

	int GetSize();

	void RemoveItem(int index);

	


};

